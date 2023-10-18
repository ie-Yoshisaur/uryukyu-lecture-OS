package worker;

import java.util.concurrent.SynchronousQueue;

public class Worker implements Runnable {
	
	public SynchronousQueue<Task> queue;
	
	public Worker(SynchronousQueue<Task> queue) {
		this.queue = queue;
	}

	@Override
	public void run() {
		while(true) {
			Task t;
			try {
				t = queue.take();
				t.work();
			} catch (InterruptedException e1) {
				e1.printStackTrace();
			}
		}
	}

}
