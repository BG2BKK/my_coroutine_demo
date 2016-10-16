coroutine demo

* 实现一个简易状态机： STATE_ENTRY, STATE_S1, STATE_S2, STATE_DONE

* 状态切换信号从标准输入读取，信号SIGNAL_A, SIGNAL_B, SIGNAL_C分别是数字0、1、2

* 状态切换表为：
	* STATE_ENTRY / SIGNAL_A -> STATE_S1
	* STATE_S1 / SIGNAL_A -> STATE_DONE
	* STATE_S1 / SIGNAL_B -> STATE_S2
	* STATE_S2 / SIGNAL_A -> STATE_ENTRY
	* STATE_S2 / SIGNAL_C -> STATE_DONE

* 除状态切换表内的定义外，FSM接收到其他信号，保持原状，不跳转
* 跳转到STATE_DONE后，FSM结束

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
