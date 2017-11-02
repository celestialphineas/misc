#ifndef MODULE
#define MODULE
#endif
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>

#define MODULE__LICENSE     "GPL"
#define MODULE__AUTHOR      "Celestial Phineas @ ZJU"
#define MODULE__DESCRIPTION "A module to get processes statuses"
#define MODULE__VERSION     "0.1"

/* According to the kernel definition (include/linux/sched.h):
 * Unrunable:   task->state <  0
 * Runable:     task->state == 0
 * Stopped:     task->state >  0
 * #define TASK_RUNNING             0x0000
 * #define TASK_INTERRUPTIBLE       0x0001
 * #define TASK_UNINTERRUPTIBLE     0x0002
 * #define __TASK_STOPPED           0x0004
 * #define __TASK_TRACED            0x0008
 */
/* #define task_is_running(task)           (task->state == TASK_RUNNING) */
static inline int task_is_running(const struct task_struct *p)
{
    return p->state == TASK_RUNNING;
}
/* #define task_is_interruptible(task)     (task->state == TASK_INTERRUPTIBLE) */
static inline int task_is_interruptible(const struct task_struct *p)
{
    return p->state == TASK_INTERRUPTIBLE;
}
/* #define task_is_uninterruptible(task)   (task->state == TASK_UNINERRUPTIBLE) */
static inline int task_is_uninterruptible(const struct task_struct *p)
{
    return p->state == TASK_UNINTERRUPTIBLE;
}
/* There's no such thing called "TASK_ZOMBIE" in fact, as far as I can see*/
/* See static inline int pid_alive(struct task); */
static inline int task_is_zombie(const struct task_struct *p)
{
    return p->pids[PIDTYPE_PID].pid == NULL;
}

static int __init myps_init(void)
{
    int running_count = 0, interruptible_count = 0, uninterruptible_count = 0,
        zombie_count = 0, stopped_count = 0, traced_count = 0;
    struct task_struct *p;
    printk(KERN_EMERG "Hello World!\n");
    for(p = &init_task; (p = next_task(p)) != &init_task;)
    {
        /* See definitions of the below inline function above */
        /* I change the def from a macro to an inline which is available in C99 */
        if(task_is_running(p))          running_count++;
        if(task_is_interruptible(p))    interruptible_count++;
        if(task_is_uninterruptible(p))  uninterruptible_count++;
        if(task_is_zombie(p))           zombie_count++;
        /* task_is_stopped is a macro definition in sched.h */
        /* #define task_is_stopped(task)    ((task->state & __TASK_STOPPED) != 0) */
        if(task_is_stopped(p))          stopped_count++;
        /* task_is_traced is also a macro definition in sched.h */
        /* #define task_is_traced(task)		((task->state & __TASK_TRACED) != 0) */
        if(task_is_traced(p))           traced_count++;
        printk(KERN_EMERG "%d %s\n", p->pid, p->comm);
    }
    printk(KERN_EMERG "Running: %d   Interruptible: %d   Uninterruptible: %d\n",
        running_count, interruptible_count, uninterruptible_count);
    printk(KERN_EMERG "Zombie: %d   Stopped: %d   Traced: %d\n",
        zombie_count, stopped_count, traced_count);
    return 0;
}

static void __exit myps_exit(void)
{
    printk(KERN_EMERG "Bye World!\n");
}

/* Initialize function and exit function, defined macros in <linux/init.h> */
module_init(myps_init);
module_exit(myps_exit);

/* Module info, parameters defined with macros in this file */
MODULE_LICENSE(MODULE__LICENSE);
MODULE_AUTHOR(MODULE__AUTHOR);
MODULE_DESCRIPTION(MODULE__DESCRIPTION);
MODULE_VERSION(MODULE__VERSION);
