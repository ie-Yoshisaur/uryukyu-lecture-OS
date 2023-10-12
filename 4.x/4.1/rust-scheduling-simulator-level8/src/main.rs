use rand::Rng;
use std::collections::BinaryHeap;
use std::thread;
use std::time::Duration;

// Define the `Task` struct, representing a task with a name and the number of clock cycles it consumes.
#[derive(Eq, PartialEq, Clone)]
struct Task {
    name: String,
    clock_cycles: u32,
    period: u32,
}

// Implement Ord and PartialOrd to use Task in a BinaryHeap (for SJF scheduling).
impl Ord for Task {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        other.period.cmp(&self.period)
    }
}

impl PartialOrd for Task {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}

// Define a trait for scheduling algorithms
trait Scheduler {
    fn push(&mut self, task: Task);
    fn pop(&mut self) -> Option<Task>;
}

// RMS scheduler
struct RMSScheduler(BinaryHeap<Task>);

impl Scheduler for RMSScheduler {
    fn push(&mut self, task: Task) {
        self.0.push(task);
    }

    fn pop(&mut self) -> Option<Task> {
        self.0.pop()
    }
}

fn task_exec<S: Scheduler>(mut scheduler: S) {
    while let Some(task) = scheduler.pop() {
        println!(
            "Executing {}: {} clock cycles, period: {}",
            task.name, task.clock_cycles, task.period
        );
        // Simulate clock cycle consumption with sleep
        thread::sleep(Duration::from_millis(task.clock_cycles.into()));
    }
}

fn main() {
    let tasks: Vec<Task> = (0..10)
        .map(|i| Task {
            name: format!("Task_{}", i),
            clock_cycles: rand::thread_rng().gen_range(1..101),
            period: 100 + rand::thread_rng().gen_range(1..101),
        })
        .collect();

    println!("RMS Scheduler:");
    let rms_scheduler = RMSScheduler(tasks.iter().cloned().collect());
    task_exec(rms_scheduler);
}
