/* The Queue manager! */

/* Remove this */
/* #include<stdio.h> */

/* NULL is in stdio.h.  Redefining NULL gives a warning, so call it null
   instead. */
#define null 0
#define NUM_REGS 3
#define MAX_PROCESSES 20
struct process_control_block {
  int pid;   /* Process ID */
  int psw;   /* Program status word */
  int page_table;   /* Pagetable info */
  int regs[NUM_REGS];   /* Array of registers */
  struct process_control_block *next;
  struct process_control_block *prev;
  int empty;
};

struct process_control_block process[MAX_PROCESSES];
struct process_control_block *head = null;
struct process_control_block *tail = null;

void init();
struct process_control_block *find_nonempty();
int enqueue(int pid, int psw, int page_table, int *regs);


/*CHANGED*/
/*Process must exist for clear function*/
void clear(struct process_control_block *process);
struct process_control_block *find_process(int pid);
int dequeue();
int delete(int id);
/*ENDCHANGE*/

void init() {
  int i = 0;
  for (i = 0; i < MAX_PROCESSES; i++) {
  process[i].empty = 1;
 }
}

struct process_control_block *find_nonempty() {
  int i = 0;
  for (i = 0; i < MAX_PROCESSES; i++) {
    if (process[i].empty == 1) {
      return &process[i];
    }
  }
  return null;
}

int enqueue(int pid, int psw, int page_table, int *regs) {
  /* Enqueue */
  struct process_control_block *newprocess = find_nonempty();
  int i = 0;

  if (!newprocess) {
    /* The queue if full */
    return(-1);
  }

  newprocess->pid = pid;
  newprocess->psw = psw;
  newprocess->page_table = page_table;
  for (i = 0; i < NUM_REGS; i++) {
      newprocess->regs[i] = regs[i];
    }

  if (tail) {
    /* The queue already has elements */
    newprocess->next = tail;
    tail->prev = newprocess;
  } else {
    /* This is the first element of the queue */
    head = newprocess;
  }
  tail = newprocess;
  newprocess->empty = 0;

  return(0);
}



/*CHANGE*/
/*Process must exist for clear function*/
void clear(struct process_control_block *process){
	process->pid = 0;
	process->psw = 0;
	process->page_table = 0;
	for (i = 0; i < NUM_REGS; i++) {
      process->regs[i] = 0;
    }
	process->next = NULL;
	process->prev = NULL;
	/*Set as empty*/
	process->empty = 0;
}


int dequeue(){
	/*If queue is empty*/
	if (head->empty == 0){
		return -1;
	}
	int ret = head->pid;
	struct process_control_block *temp = head;
	/*If entry is only one in queue*/
	if(head->prev == NULL){
		head = NULL;
		tail = NULL;
	}
	else{
		/*Reset head pointer*/
		head = head->prev;
	}
	clear(temp);
	return(ret);
}
struct process_control_block *find_process(int id){
	struct process_control_block temp = tail;
	while(temp != NULL){
		if(id == temp->pid){
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

int delete(int id){
	struct process_control_block *temp = find_process(id);
	/*If process doesnt exist*/
	if (temp == NULL){
		return -1;
	}
	int ret = temp->pid;
	/*If entry is only one in queue*/
	if(temp->next == NULL && temp->prev == NULL){
		head = NULL;
		tail = NULL;
	}
	/*If entry is at tail*/
	else if(temp->prev == NULL){
		tail = temp->next;
	}
	/*If entry is at head*/
	
	
}

/*ENDCHANGE*/
