package worker;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.SynchronousQueue;

public class ThreadPoolTest {
	static public void main(String [] arg) {
		int cpu = 16;
		System.out.println("hello");
		ExecutorService pool = Executors.newFixedThreadPool(cpu);
		System.out.println(pool);
		SynchronousQueue<Task> queue = new SynchronousQueue<Task>();
		System.out.println(queue);
		
		for(int i =0;i<cpu;i++) {
			pool.execute(new Worker(queue));
		}
		
		new Thread(new Runnable(){
			@Override
			public void run() {
				while(true) {
					try {
						Thread.sleep(50);
						System.out.println(); // " chick");
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		}).start();
		String [] data = {
				"a","b","c","d","e","f",
				"a","b","c","d","e","f",
				"a","b","c","d","e","f",
				"a","b","c","d","e","f",
				"a","b","c","d","e","f",
				"a","b","c","d","e","f",
				"a","b","c","d","e","f",
				"a","b","c","d","e","f",
		""};
		for( String s : data) {
			try {
				queue.put(new Task(s));
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		
		
		
	}
}

