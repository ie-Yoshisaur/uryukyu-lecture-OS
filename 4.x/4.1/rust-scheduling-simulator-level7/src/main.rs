use rand::Rng;
use std::cmp::Reverse;
use std::collections::BinaryHeap;
use std::collections::VecDeque;
use std::thread;
use std::time::Duration;
use svg::node::element::Rectangle;
use svg::node::element::Text;
use svg::Document;

// Define the `Task` struct, representing a task with a name and the number of clock cycles it consumes.
#[derive(Eq, PartialEq, Clone)]
struct Task {
    name: String,
    clock_cycles: u32,
}

// Implement Ord and PartialOrd to use Task in a BinaryHeap (for SJF scheduling).
impl Ord for Task {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        self.clock_cycles.cmp(&other.clock_cycles)
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

// FIFO Scheduler
#[derive(Clone)]
struct FIFOScheduler(VecDeque<Task>);

impl Scheduler for FIFOScheduler {
    fn push(&mut self, task: Task) {
        self.0.push_back(task);
    }

    fn pop(&mut self) -> Option<Task> {
        self.0.pop_front()
    }
}

// SJF Scheduler
#[derive(Clone)]
struct SJFScheduler(BinaryHeap<Reverse<Task>>);

impl Scheduler for SJFScheduler {
    fn push(&mut self, task: Task) {
        self.0.push(Reverse(task));
    }

    fn pop(&mut self) -> Option<Task> {
        self.0.pop().map(|reverse| reverse.0)
    }
}

// Round Robin Scheduler
#[derive(Clone)]
struct RRScheduler {
    queue: VecDeque<Task>,
    quantum: u32,
}

impl Scheduler for RRScheduler {
    fn push(&mut self, task: Task) {
        self.queue.push_back(task);
    }

    fn pop(&mut self) -> Option<Task> {
        if let Some(mut task) = self.queue.pop_front() {
            if task.clock_cycles > self.quantum {
                task.clock_cycles -= self.quantum;
                self.queue.push_back(task.clone());
                Some(Task {
                    name: task.name,
                    clock_cycles: self.quantum,
                })
            } else {
                Some(task)
            }
        } else {
            None
        }
    }
}

fn task_exec<S: Scheduler>(mut scheduler: S) {
    while let Some(task) = scheduler.pop() {
        println!(
            "Executing {}: {} clock cycles",
            task.name, task.clock_cycles
        );
        // Simulate clock cycle consumption with sleep
        thread::sleep(Duration::from_millis(task.clock_cycles.into()));
    }
}

fn generate_gantt_chart<S: Scheduler>(mut scheduler: S, filename: &str) {
    let mut document = Document::new().set("viewBox", (0, 0, 600, 100));
    let mut current_time = 0;

    while let Some(task) = scheduler.pop() {
        let task_duration = task.clock_cycles;
        let end_time = current_time + task_duration;
        let task_name_segments: Vec<&str> = task.name.split('_').collect();
        let task_index = task_name_segments[1].parse::<usize>().unwrap();
        let current_row = task_index;

        // create a rectangle for the task duration
        let rect = Rectangle::new()
            .set("x", current_time)
            .set("y", current_row * 15)
            .set("width", task_duration)
            .set("height", 15)
            .set("fill", "green");

        // text for the task's name
        let text = Text::new()
            .set("x", end_time)
            .set("y", current_row * 15 + 5)
            .set("font-size", 5)
            .add(svg::node::Text::new(task.name));

        // text for the current time
        let time_text = Text::new()
            .set("x", end_time)
            .set("y", current_row * 15 + 10)
            .set("font-size", 5)
            .add(svg::node::Text::new(format!("Start: {}", current_time)));

        // text for the end time
        let end_time_text = Text::new()
            .set("x", end_time)
            .set("y", current_row * 15 + 15)
            .set("font-size", 5)
            .add(svg::node::Text::new(format!("End: {}", end_time)));

        document = document
            .add(rect)
            .add(text)
            .add(time_text)
            .add(end_time_text);

        current_time += task_duration;
    }

    svg::save(filename, &document).unwrap();
}

fn main() {
    // Create tasks. VecDeque allows efficient popping and pushing operations.
    let tasks: Vec<Task> = (0..10)
        .map(|i| {
            Task {
                name: format!("Task_{}", i),
                clock_cycles: rand::thread_rng().gen_range(1..101), // Random number between 1 and 100
            }
        })
        .collect();

    println!("FIFO Scheduler:");
    let fifo_scheduler = FIFOScheduler(VecDeque::from(tasks.clone()));
    task_exec(fifo_scheduler.clone());
    generate_gantt_chart(fifo_scheduler, "fifo_scheduler.svg");

    println!("\nSJF Scheduler:");
    let sjf_scheduler = SJFScheduler(tasks.iter().cloned().map(Reverse).collect());
    task_exec(sjf_scheduler.clone());
    generate_gantt_chart(sjf_scheduler, "sjf_scheduler.svg");

    println!("\nRound Robin Scheduler:");
    let rr_scheduler = RRScheduler {
        queue: VecDeque::from(tasks),
        quantum: 20,
    };
    task_exec(rr_scheduler.clone());
    generate_gantt_chart(rr_scheduler, "rr_scheduler.svg");
}
