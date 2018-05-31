#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/delay.h>
#include <linux/uaccess.h>

static unsigned int variable;
static struct proc_dir_entry *test_dir, *test_entry;

static noinline void kill_moretime(void) {
	mdelay(2);
}

static noinline void kill_time(void) {
	mdelay(2);
	kill_moretime();
}

static int test_proc_show(struct seq_file *seq, void *v) {
	unsigned int *ptr_var = seq->private;
	kill_time();
	seq_printf(seq, "%u\n", *ptr_var);
	return 0;
}

static ssize_t test_proc_write(struct file *file, const char __user *buffer,
                               size_t count, loff_t *ppos) {
	struct seq_file *seq = file->private_data;
	unsigned int *ptr_var = seq->private;
	int err;
	char *kbuffer;

	if (!buffer || count > PAGE_SIZE - 1)
		return -EINVAL;

	kbuffer = (char *)__get_free_page(GFP_KERNEL);
	if (!kbuffer)
		return -ENOMEM;

	err = -EFAULT;
	if (copy_from_user(kbuffer, buffer, count))
		goto out;
	kbuffer[count] = '\0';

	*ptr_var = simple_strtoul(kbuffer, NULL, 10);
	return count;

out:
	free_page((unsigned long)buffer);
	return err;
}

static int test_proc_open(struct inode *inode, struct file *file) {
	return single_open(file, test_proc_show, PDE_DATA(inode));
}

static const struct file_operations test_proc_fops = {
	.owner = THIS_MODULE,
	.open = test_proc_open,
	.read = seq_read,
	.write = test_proc_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static __init int test_proc_init(void) {
	test_dir = proc_mkdir("test_dir", NULL);
	if (test_dir) {
		test_entry = proc_create_data("test_rw",0666, test_dir, &test_proc_fops, &variable);
		if (test_entry)
			return 0;
	}

	return -ENOMEM;
}
module_init(test_proc_init);

static __exit void test_proc_cleanup(void) {
	remove_proc_entry("test_rw", test_dir);
	remove_proc_entry("test_dir", NULL);
}
module_exit(test_proc_cleanup);

MODULE_AUTHOR("Barry Song <baohua@kernel.org>");
MODULE_DESCRIPTION("proc exmaple");
MODULE_LICENSE("GPL v2");

/*
// http://mp.weixin.qq.com/s/aFpXGrQ7sHaZguL66QF-tA

insmod proc.ko

./function.sh cat /proc/test_dir/test_rw



 cat /sys/kernel/debug/tracing/trace
# tracer: function_graph
#
# CPU  DURATION                  FUNCTION CALLS
# |     |   |                     |   |   |   |
 1)               |  test_proc_show [proc]() {
 1)               |    kill_time [proc]() {
 1)               |      delay_tsc() {
 1)   ==========> |
 1)               |        smp_apic_timer_interrupt() {
 1)               |          irq_enter() {
 1)   0.105 us    |            rcu_irq_enter();
 1)   0.820 us    |          }
 1)   0.061 us    |          exit_idle();
 1)   0.337 us    |          kvm_guest_apic_eoi_write();
 1)               |          local_apic_timer_interrupt() {
 1)               |            hrtimer_interrupt() {
 1)   0.145 us    |              _raw_spin_lock();
 1)   0.282 us    |              ktime_get_update_offsets_now();
 1)               |              __hrtimer_run_queues() {
 1)   0.401 us    |                __remove_hrtimer();
 1)               |                tick_sched_timer() {
 1)   0.102 us    |                  ktime_get();
 1)   0.051 us    |                  tick_sched_do_timer();
 1)               |                  tick_sched_handle.isra.14() {
 1)               |                    update_process_times() {
 1)               |                      account_process_tick() {
 1)   0.104 us    |                        kvm_steal_clock();
 1)   0.050 us    |                        nsecs_to_jiffies();
 1)   0.050 us    |                        jiffies_to_usecs();
 1)               |                        account_system_time() {
 1)   1.725 us    |                          cpuacct_account_field();
 1)               |                          acct_account_cputime() {
 1)               |                            __acct_update_integrals() {
 1)   0.049 us    |                              jiffies_to_timeval();
 1)   0.898 us    |                            }
 1)   1.342 us    |                          }
 1)   4.240 us    |                        }
 1)   6.368 us    |                      }
 1)   0.063 us    |                      hrtimer_run_queues();
 1)   0.068 us    |                      raise_softirq();
 1)               |                      rcu_check_callbacks() {
 1)   0.050 us    |                        cpu_needs_another_gp();
 1)   0.067 us    |                        raise_softirq();
 1)   1.835 us    |                      }
 1)               |                      scheduler_tick() {
 1)   0.057 us    |                        _raw_spin_lock();
 1)   0.104 us    |                        update_rq_clock.part.77();
 1)               |                        task_tick_fair() {
 1)               |                          update_curr() {
 1)   0.067 us    |                            update_min_vruntime();
 1)   0.506 us    |                            cpuacct_charge();
 1)   1.455 us    |                          }
 1)   0.047 us    |                          __compute_runnable_contrib();
 1)               |                          update_cfs_shares() {
 1)               |                            update_curr() {
 1)   0.060 us    |                              update_min_vruntime();
 1)   0.545 us    |                            }
 1)   0.043 us    |                            account_entity_dequeue();
 1)   0.050 us    |                            account_entity_enqueue();
 1)   1.953 us    |                          }
 1)   0.530 us    |                          hrtimer_active();
 1)   0.062 us    |                          update_curr();
 1)   0.046 us    |                          __compute_runnable_contrib();
 1)               |                          update_cfs_shares() {
 1)               |                            update_curr() {
 1)   0.090 us    |                              __calc_delta();
 1)   0.050 us    |                              update_min_vruntime();
 1)   1.128 us    |                            }
 1)   0.047 us    |                            account_entity_dequeue();
 1)   0.047 us    |                            account_entity_enqueue();
 1)   2.618 us    |                          }
 1)   0.285 us    |                          hrtimer_active();
 1)   0.061 us    |                          update_curr();
 1)   0.046 us    |                          __compute_runnable_contrib();
 1)               |                          update_cfs_shares() {
 1)               |                            update_curr() {
 1)   0.132 us    |                              __calc_delta();
 1)   0.048 us    |                              update_min_vruntime();
 1) + 15.515 us   |                            }
 1)   0.068 us    |                            account_entity_dequeue();
 1)   0.072 us    |                            account_entity_enqueue();
 1) + 16.807 us   |                          }
 1)   0.282 us    |                          hrtimer_active();
 1)   0.050 us    |                          update_curr();
 1)   0.048 us    |                          __compute_runnable_contrib();
 1)   0.108 us    |                          update_cfs_shares();
 1)   0.050 us    |                          hrtimer_active();
 1) + 30.939 us   |                        }
 1)               |                        update_cpu_load_active() {
 1)               |                          __update_cpu_load() {
 1)   0.080 us    |                            sched_avg_update();
 1)   0.505 us    |                          }
 1)   0.983 us    |                        }
 1)   0.060 us    |                        calc_global_load_tick();
 1)               |                        trigger_load_balance() {
 1)   0.065 us    |                          raise_softirq();
 1)   1.087 us    |                        }
 1) + 35.774 us   |                      }
 1)   0.350 us    |                      run_posix_cpu_timers();
 1) + 47.394 us   |                    }
 1)   0.235 us    |                    profile_tick();
 1) + 48.365 us   |                  }
 1)   0.087 us    |                  hrtimer_forward();
 1) + 50.061 us   |                }
 1)   0.050 us    |                _raw_spin_lock();
 1)   0.332 us    |                enqueue_hrtimer();
 1) + 52.587 us   |              }
 1)   0.075 us    |              __hrtimer_get_next_event();
 1)               |              tick_program_event() {
 1)               |                clockevents_program_event() {
 1)   0.189 us    |                  ktime_get();
 1)   1.986 us    |                  lapic_next_event();
 1)   2.976 us    |                }
 1)   3.372 us    |              }
 1) + 58.469 us   |            }
 1) + 58.987 us   |          }
 1)               |          irq_exit() {
 1)               |            __do_softirq() {
 1)               |              run_timer_softirq() {
 1)   0.048 us    |                _raw_spin_lock_irq();
 1)   0.724 us    |              }
 1)               |              run_rebalance_domains() {
 1)               |                rebalance_domains() {
 1)               |                  update_blocked_averages() {
 1)   0.165 us    |                    _raw_spin_lock_irqsave();
 1)               |                    update_rq_clock() {
 1)   0.071 us    |                      update_rq_clock.part.77();
 1)   0.432 us    |                    }
 1)   0.048 us    |                    __compute_runnable_contrib();
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.412 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.050 us    |                      __compute_runnable_contrib.part.57();
 1)   0.390 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.403 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.382 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.049 us    |                      __compute_runnable_contrib.part.57();
 1)   0.382 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.412 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.050 us    |                      __compute_runnable_contrib.part.57();
 1)   0.522 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.532 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.514 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.050 us    |                      __compute_runnable_contrib.part.57();
 1)   0.551 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.049 us    |                      __compute_runnable_contrib.part.57();
 1)   0.382 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.049 us    |                      __compute_runnable_contrib.part.57();
 1)   0.386 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.414 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.416 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.050 us    |                      __compute_runnable_contrib.part.57();
 1)   0.384 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.050 us    |                      __compute_runnable_contrib.part.57();
 1)   0.390 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.385 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.050 us    |                      __compute_runnable_contrib.part.57();
 1)   0.389 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.049 us    |                      __compute_runnable_contrib.part.57();
 1)   0.385 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.050 us    |                      __compute_runnable_contrib.part.57();
 1)   0.422 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.383 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.049 us    |                      __compute_runnable_contrib.part.57();
 1)   0.389 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.384 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.389 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.387 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.422 us    |                    }
 1)   0.057 us    |                    __compute_runnable_contrib();
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.395 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.380 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.386 us    |                    }
 1)               |                    __compute_runnable_contrib() {
 1)   0.048 us    |                      __compute_runnable_contrib.part.57();
 1)   0.387 us    |                    }
 1)   0.062 us    |                    _raw_spin_unlock_irqrestore();
 1) + 38.740 us   |                  }
 1)   0.049 us    |                  __msecs_to_jiffies();
 1) + 40.791 us   |                }
 1) + 41.695 us   |              }
 1)               |              rcu_process_callbacks() {
 1)               |                note_gp_changes() {
 1)   0.060 us    |                  _raw_spin_trylock();
 1)               |                  __note_gp_changes() {
 1)               |                    rcu_advance_cbs() {
 1)   0.410 us    |                      rcu_accelerate_cbs();
 1)   0.908 us    |                    }
 1)   1.387 us    |                  }
 1)   0.060 us    |                  _raw_spin_unlock_irqrestore();
 1)   2.882 us    |                }
 1)   0.159 us    |                cpu_needs_another_gp();
 1)   0.169 us    |                note_gp_changes();
 1)   0.514 us    |                cpu_needs_another_gp();
 1)   5.394 us    |              }
 1)   0.060 us    |              rcu_bh_qs();
 1)   0.049 us    |              __local_bh_enable();
 1) + 50.329 us   |            }
 1)   0.049 us    |            idle_cpu();
 1)   0.200 us    |            rcu_irq_exit();
 1) + 51.616 us   |          }
 1) ! 114.183 us  |        }
 1)   <========== |
 1) # 1984.392 us |      }
 1)               |      kill_moretime [proc]() {
 1) # 1984.217 us |        delay_tsc();
 1) # 1985.092 us |      }
 1) # 3971.558 us |    }
 1)               |    seq_printf() {
 1)   1.525 us    |      seq_vprintf();
 1)   2.017 us    |    }
 1) # 3975.158 us |  }
*/
