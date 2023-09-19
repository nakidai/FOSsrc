
struct regs
{
    unsigned int gs, fs, es, ds;      
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  
    unsigned int int_no, err_code;    
    unsigned int eip, cs, eflags, useresp, ss;    
};


extern void irq0();
extern void irq1();    
extern void irq14();             
extern void irq15();


void *irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};


void irq_install_handler(int irq, void (*handler)(struct regs *r))
{
    irq_routines[irq] = handler;
}


void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}


void irq_remap(void)
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}


void irq_install()
{
    irq_remap();

    idt_set_gate(32, (unsigned)irq0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned)irq1, 0x08, 0x8E);
    idt_set_gate(46, (unsigned)irq14, 0x08, 0x8E);
         
    idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);
}


void irq_handler(struct regs *r)
{
    
    void (*handler)(struct regs *r);

    
    handler = irq_routines[r->int_no - 32];
    if (handler)
    {
        handler(r);
    }

    
    if (r->int_no >= 40)
    {
        outb(0xA0, 0x20);
    }

    
    outb(0x20, 0x20);
}
		