coroutine demo

* 使用协程实现有限状态机FSM，伪码设计如下：

```cpp

fsm state transmission procedure:
	// 执行action
	// 实现状态跳转
	// 切换回原执行序列

	fsm_trans_func(sig) {
		static FSM_STATE state = STATE_ENTRY;
		switch(state) {
			case STATE_ENTRY:
				
				switch(sig) {
					case SIGNAL_A:
						action_A();
						state = STATE_S1;
						yield(state);
				}
		}
	}


resume procedure:
	// 接受输入参数，重入fsm

yield procedure
	// 更新fsm状态后，让出cpu，切回原执行序列


main procedure:

	C *fsm = create();
	while(1) {
		signal sig = get_input_signal();
		state curr = resume(fsm, sig);
		if(curr == STATE_DONE) {
			print 'FSM stop'
			break;
		}
	}

	del(fsm);

```
