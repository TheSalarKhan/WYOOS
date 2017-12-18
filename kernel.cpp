
extern "C" void kernel_main()
{
	printf("Hello World");
	
	// At the end of everything, we enter an
	// infinite loop Because:
	// 1) Control should not be here in the first place -_-
	// 2) But now that we are here. We cannot simply do nothing,
	// that's just not how computers work.
	while(1);
}
