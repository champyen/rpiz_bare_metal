
void printf(const char *fmt, ...);

void bare_metal_start(void)
{
    printf("\n\n%s: Hello World! %s %s %d\n\n", __func__, __DATE__, __TIME__, __LINE__);
    printf("enter busy loop\n");
    while(1);
}