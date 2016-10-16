coroutine demo

* 使用协程实现有限状态机FSM，伪码设计如下：

```cpp

fsm state transmission procedure:

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

yield procedure


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
