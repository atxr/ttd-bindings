#include <windows.h> 
#include <stdio.h> 
#include <iostream>
#include <strsafe.h>
#include "TTD.hpp"

void callCallback_tree(unsigned __int64 callback_value, TTD::GuestAddress addr_func, TTD::GuestAddress addr_ret, struct TTD::TTD_Replay_IThreadView* thread_view) {
	bool print = *(bool*)callback_value;
	if (print) {
		printf("[CALLBACK TRIGGERED]\n");
		*(bool *)callback_value = false;
	}
}

int main() {
	TTD::ReplayEngine ttdengine = TTD::ReplayEngine(L"C:\\Users\\Alexandre\\Documents\\ttd\\dll\\TTDReplay.dll", L"C:\\Users\\Alexandre\\Documents\\ttd\\dll\\TTDReplayCPU.dll");
	TTD::TTD_Replay_ICursorView_ReplayResult replayrez;
	int result;

	result = ttdengine.Initialize(L"C:\\Users\\Alexandre\\Documents\\ttd\\traces\\Notepad01.run");
	if (result == 0) {
		std::wcerr << "Fail to open the trace";
		exit(-1);
	}

	TTD::Cursor ttdcursor = ttdengine.NewCursor();
	TTD::Position* first = ttdengine.GetFirstPosition();
	TTD::Position* last = ttdengine.GetLastPosition();

	// Set the callback
	bool print;
	ttdcursor.SetCallReturnCallback((TTD::PROC_CallCallback)callCallback_tree, (unsigned long long) & print);

	// Test 1: with a fixed number of steps
	ttdcursor.SetPosition(first);
	print = true;
	printf("\nStart test 1:\n");
	ttdcursor.ReplayForward(&replayrez, last, 100);
	printf("\nEnd test 1: ");
	if (!print)
		printf("Success!\n");
	else
		printf("Fail!\n");

	// Test 2: with a step of -1
	ttdcursor.SetPosition(first);
	print = true;
	printf("\nStart test 2:\n");
	ttdcursor.ReplayForward(&replayrez, last, -1);
	printf("\nEnd test 2: ");
	if (!print)
		printf("Success!\n");
	else
		printf("Fail!\n");


	// Test 3: with a while loop and a step of 1
	ttdcursor.SetPosition(first);
	print = true;
	printf("\nStart test 3 (a bit long):\n");
	do {
		ttdcursor.ReplayForward(&replayrez, last, 1);
	} while (replayrez.stepCount);
	printf("\nEnd test 3: ");
	if (!print)
		printf("Success!\n");
	else
		printf("Fail!\n");
}
