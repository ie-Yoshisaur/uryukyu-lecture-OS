use rand::Rng;
use std::cmp::Reverse;
use std::collections::BinaryHeap;
use std::collections::VecDeque;
use std::sync::{Arc, Condvar, Mutex};
use std::thread;

#[derive(Clone)]
struct Task {
    name: String,
    clock_cycles: u32,
    pair: Arc<(Mutex<bool>, Condvar)>,
}

impl Eq for Task {}

impl PartialEq for Task {
    fn eq(&self, other: &Self) -> bool {
        self.clock_cycles == other.clock_cycles
    }
}

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

impl Task {
    fn new(name: String, clock_cycles: u32) -> Self {
        Self {
            name,
            clock_cycles,
            pair: Arc::new((Mutex::new(false), Condvar::new())),
        }
    }

    fn execute(&self) {
        let pair_clone = self.pair.clone();
        let name = self.name.clone();
        let clock_cycles = self.clock_cycles;
        thread::spawn(move || {
            let (lock, cvar) = &*pair_clone;
            let mut started = lock.lock().unwrap();
            while !*started {
                started = cvar.wait(started).unwrap();
            }
            println!("Executing {}: {} clock cycles", name, clock_cycles);
        });

        let (lock, cvar) = &*self.pair;
        let mut started = lock.lock().unwrap();
        *started = true;
        cvar.notify_one();
    }
}

// FIFO Scheduler
struct FIFOScheduler {
    tasks: VecDeque<Task>,
}

impl FIFOScheduler {
    fn new() -> Self {
        Self {
            tasks: VecDeque::new(),
        }
    }

    fn push(&mut self, task: Task) {
        self.tasks.push_back(task);
    }

    fn pop(&mut self) -> Option<Task> {
        self.tasks.pop_front()
    }
}

struct SJFScheduler {
    tasks: BinaryHeap<Reverse<Task>>,
}

impl SJFScheduler {
    fn new() -> Self {
        Self {
            tasks: BinaryHeap::new(),
        }
    }

    fn push(&mut self, task: Task) {
        self.tasks.push(Reverse(task));
    }

    fn pop(&mut self) -> Option<Task> {
        self.tasks.pop().map(|reverse| reverse.0)
    }
}

// Round Robin Scheduler
struct RRScheduler {
    tasks: VecDeque<Task>,
    quantum: u32,
}

impl RRScheduler {
    fn new(quantum: u32) -> Self {
        Self {
            tasks: VecDeque::new(),
            quantum,
        }
    }

    fn push(&mut self, task: Task) {
        self.tasks.push_back(task);
    }

    fn pop(&mut self) -> Option<Task> {
        if let Some(mut task) = self.tasks.pop_front() {
            if task.clock_cycles > self.quantum {
                task.clock_cycles -= self.quantum; // reduce the remaining clock cycles
                let remaining_task = task.clone();
                self.tasks.push_back(remaining_task); // re-add to the queue
                task.clock_cycles = self.quantum; // execute only the quantum amount
            }
            Some(task)
        } else {
            None
        }
    }
}

fn main() {
    let tasks: Vec<Task> = (1..=5)
        .map(|i| {
            let clock_cycles = rand::thread_rng().gen_range(1..=100);
            Task::new(format!("Task_{}", i), clock_cycles)
        })
        .collect();

    println!("FIFO Scheduler:");
    let mut fifo_scheduler = FIFOScheduler::new();
    for task in tasks.clone() {
        fifo_scheduler.push(task);
    }
    while let Some(task) = fifo_scheduler.pop() {
        task.execute();
        thread::sleep(std::time::Duration::from_millis(task.clock_cycles.into()));
    }

    println!("\nSJF Scheduler:");
    let mut sjf_scheduler = SJFScheduler::new();
    for task in tasks.clone() {
        sjf_scheduler.push(task);
    }
    while let Some(task) = sjf_scheduler.pop() {
        task.execute();
        thread::sleep(std::time::Duration::from_millis(task.clock_cycles.into()));
    }

    println!("\nRound Robin Scheduler:");
    let mut rr_scheduler = RRScheduler::new(20);
    for task in tasks {
        rr_scheduler.push(task);
    }
    while let Some(task) = rr_scheduler.pop() {
        task.execute();
        thread::sleep(std::time::Duration::from_millis(task.clock_cycles.into()));
    }
}
