package worker;

public class Task {

	private String s;

	public Task(String s) {
		this.s = s;
	}
	
	public String get() {
		return s;
	}

	public void work() throws InterruptedException {
			Thread.sleep(100);
		System.out.print(s);
	}

}
