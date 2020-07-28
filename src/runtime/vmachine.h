typedef struct RuntimeStack RuntimeStack;

typedef struct VMachine VMachine;

typedef struct Property Property;

typedef struct Argument Argument;

extern VMachine *vmachine;

struct VMachine {

    ArrayList *classListPool;
    
    HashMap *runtimeClassPool;

    HashMap *userDefineClassPool;

    Class *mainEntryClass;

    Class* (*findClassByName)(VMachine *this, char* name);

    void (*startVMachine)(VMachine *this);

    void (*initMainEntryClass)(VMachine *this, char *classname);

    void (*initClassFromRuntimeJarFile)(VMachine *this);

    void (*initClassFromUserDefineJarFile)(VMachine *this, char *filename);

    void (*linkClassPoolConfig)(VMachine *this);
};

VMachine* buildVMachine();
