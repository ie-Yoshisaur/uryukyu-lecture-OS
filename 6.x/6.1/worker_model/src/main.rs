use std::error::Error;
use std::sync::Arc;
use tokio::sync::{mpsc, Mutex};

#[derive(Clone)]
struct Task {
    s: Arc<String>,
}

impl Task {
    // Constructs a new `Task`.
    fn new(s: Arc<String>) -> Self {
        Task { s }
    }

    // Simulates work asynchronously.
    async fn work(&self) {
        tokio::time::sleep(tokio::time::Duration::from_millis(100)).await;
        println!("{}", self.s);
    }
}

struct Worker {
    receiver: Arc<Mutex<mpsc::Receiver<Task>>>,
}

impl Worker {
    // Constructs a new `Worker`.
    fn new(receiver: Arc<Mutex<mpsc::Receiver<Task>>>) -> Self {
        Worker { receiver }
    }

    // Runs the worker, listening for new tasks.
    async fn run(&self) {
        // `lock().await` is used to acquire the lock asynchronously.
        while let Some(task) = self.receiver.lock().await.recv().await {
            task.work().await;
        }
    }
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    let cpu_count = 16;
    let (tx, rx) = mpsc::channel::<Task>(cpu_count); // Buffer size can be adjusted as needed.

    let rx = Arc::new(Mutex::new(rx)); // Share Receiver among several workers.

    // Launch multiple workers.
    for _ in 0..cpu_count {
        let worker = Worker::new(rx.clone()); // Pass a clone of Arc<Mutex<Receiver>> to each worker.

        tokio::spawn(async move {
            worker.run().await;
        });
    }

    // Spawn a new thread to periodically output messages.
    tokio::spawn(async {
        loop {
            tokio::time::sleep(tokio::time::Duration::from_millis(50)).await;
            println!(); // Use println!("chick"); if you want to output "chick".
        }
    });

    let data = [
        "a", "b", "c", "d", "e", "f", "a", "b", "c", "d", "e", "f", "a", "b", "c", "d", "e", "f",
        "a", "b", "c", "d", "e", "f", "a", "b", "c", "d", "e", "f", "a", "b", "c", "d", "e", "f",
        "a", "b", "c", "d", "e", "f", "a", "b", "c", "d", "e", "f",
    ];

    // Send tasks.
    for s in &data {
        let task_message = Arc::new(s.to_string());
        let task = Task::new(task_message);
        tx.send(task).await?;
    }

    // Once all tasks are sent, drop tx to close the channel.
    // This will cause the worker's loop to terminate, ending the worker tasks as a result.
    drop(tx);

    // At this point, the main task ends, but the program continues running until all background tasks are complete.
    Ok(())
}
