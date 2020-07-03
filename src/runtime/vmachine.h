typedef struct VMachine VMachine;

typedef struct Property Property;

typedef struct Argument Argument;

struct VMachine {
    
    HashMap *runtimeClassPool;

    HashMap *userDefineClassPool;

    Class* (*findClassByName)(VMachine *this, char* name);

    void (*startVMachine)(VMachine *this);
};

VMachine* buildVMachine();
