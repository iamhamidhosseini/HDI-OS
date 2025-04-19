typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define VIDEO_MEM ((u8*)0xB8000)
#define REG_PORT 0x3D4
#define DATA_PORT 0x3D5
static int cursor_pos = 0;

static void outb(u16 port, u8 val){
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}
static u8 inb(u16 port){
    u8 ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
static void update_cursor(){
    outb(REG_PORT, 0x0F);
    outb(DATA_PORT, (u8)(cursor_pos & 0xFF));
    outb(REG_PORT, 0x0E);
    outb(DATA_PORT, (u8)((cursor_pos >> 8) & 0xFF));
}
static void put_char(char c){
    if(c == '\n'){
        int col = cursor_pos % 80;
        cursor_pos += (80 - col);
    } else {
        VIDEO_MEM[cursor_pos*2] = c;
        VIDEO_MEM[cursor_pos*2+1] = 0x07;
        cursor_pos++;
    }
    update_cursor();
}
static void put_str(const char* s){
    while(*s) put_char(*s++);
}
static char get_char(){
    while(1){
        if(inb(0x64) & 1){
            u8 sc = inb(0x60);
            static char map[128] = {0,
                ' ',0,'1','2','3','4','5','6','7','8','9','0','-','=',0,0,
                'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,'a','s',
                'd','f','g','h','j','k','l',';','\'',0,0,'\\','z','x','c','v',
                'b','n','m',',','.','/'};
            char c = map[sc];
            if(c) return c;
        }
    }
}
static int strcmp(const char *a, const char *b){
    while(*a && *a == *b){ a++; b++; }
    return *a - *b;
}
static int strncmp(const char* a, const char* b, int n){
    for(int i = 0; i < n; i++){
        if(a[i] != b[i] || !a[i] || !b[i]) return a[i] - b[i];
    }
    return 0;
}
static void strcpy(char* dst, const char* src){
    while((*dst++ = *src++));
}
static int strlen(const char* s){
    int i = 0;
    while(s[i]) i++;
    return i;
}

// ==== fake file systems ====
typedef enum { FILE_NODE, DIR_NODE } NodeType;

typedef struct Node {
    const char* name;
    NodeType type;
    const char* content;
    struct Node* children;
    int child_count;
    struct Node* parent;
} Node;

Node bin_children[] = {
    { "app", FILE_NODE, "This is a test app", 0, 0, 0 }
};
Node etc_children[] = {
    { "config.sys", FILE_NODE, "CFG=1\nMODE=HDI", 0, 0, 0 }
};
Node root_nodes[] = {
    { "bin", DIR_NODE, 0, bin_children, 1, 0 },
    { "etc", DIR_NODE, 0, etc_children, 1, 0 },
    { "readme.txt", FILE_NODE, "Welcome to HDI OS!", 0, 0, 0 }
};

Node root = { "/", DIR_NODE, 0, root_nodes, 3, 0 };
Node* current = &root;

// ==== shell prompts ====

void cmd_ls() {
    for(int i = 0; i < current->child_count; i++) {
        put_str(current->children[i].name);
        if(current->children[i].type == DIR_NODE) put_char('/');
        put_char('\n');
    }
}

void cmd_cd(const char* name) {
    if(strcmp(name, "..") == 0){
        if(current->parent) current = current->parent;
        return;
    }
    for(int i = 0; i < current->child_count; i++){
        if(current->children[i].type == DIR_NODE && strcmp(current->children[i].name, name)==0){
            current = &current->children[i];
            return;
        }
    }
    put_str("No such directory\n");
}

void cmd_cat(const char* name){
    for(int i = 0; i < current->child_count; i++){
        if(current->children[i].type == FILE_NODE && strcmp(current->children[i].name, name)==0){
            put_str(current->children[i].content);
            put_char('\n');
            return;
        }
    }
    put_str("No such file\n");
}

void cmd_echo(const char* msg){
    put_str(msg);
    put_char('\n');
}

// ==== main ====
void main(){
    // connect to parents
    for(int i=0; i<root.child_count; i++) {
        root.children[i].parent = &root;
        for(int j=0; j<root.children[i].child_count; j++)
            root.children[i].children[j].parent = &root.children[i];
    }

    put_str("welcome to HDI OS\n\n");

    char buf[64];
    while(1){
        put_str(current->name);
        put_str(" # ");

        int idx = 0;
        while(1){
            char c = get_char();
            if(c=='\n' || c=='\r') break;
            if(c=='\b' && idx>0){
                idx--;
                cursor_pos--; update_cursor();
                VIDEO_MEM[cursor_pos*2] = ' ';
                VIDEO_MEM[cursor_pos*2+1] = 0x07;
                update_cursor();
            } else if(c>=' ' && idx<63){
                buf[idx++] = c;
                put_char(c);
            }
        }
        buf[idx] = 0;
        put_char('\n');

        if(idx==0) continue;
        else if(strcmp(buf, "ls")==0){
            cmd_ls();
        } else if(strncmp(buf, "cd ", 3)==0){
            cmd_cd(buf+3);
        } else if(strncmp(buf, "cat ", 4)==0){
            cmd_cat(buf+4);
        } else if(strncmp(buf, "echo ", 5)==0){
            cmd_echo(buf+5);
        } else if(strcmp(buf, "exit")==0){
            put_str("Shutting down...\n");
            while(1); // halt
        } else {
            put_str("Unknown command\n");
        }
    }
}

void _start(){ main(); }
