// Generated from /mnt/d/workspace/code/tf2/zf/src/python/perks/test.py

#if defined __test_included
#endinput
#endif
#define __test_included

#include <datapack>

methodmap Person < DataPack {
    public Person(char[] name, int health) {
        DataPack sm_base = new DataPack();
        Person sm = view_as<Person>(sm_base);

        sm.Position = DP(0);
        sm.WriteCell(health);
        sm.Position = DP(2);
        sm.WriteString(name);
        sm.Position = DP(3);
        sm.WriteFunction(Person_speak);
        sm.Position = DP(4);
        sm.WriteFunction(Person_take_damage);

        return sm;
    }

    property int health {
        public get() {
            this.Position = DP(0);
            return this.ReadCell();
        }
        public set(int value) {
            this.Position = DP(0);
            this.WriteCell(value);
        }
    }

    property bool is_alive {
        public get() {
            this.Position = DP(1);
            return this.ReadCell();
        }
        public set(bool value) {
            this.Position = DP(1);
            this.WriteCell(value);
        }
    }

    public void getName(char[] buffer, int maxlen) {
        this.Position = DP(2);
        this.ReadString(buffer, maxlen);
    }
    public void setName(const char[] value) {
        this.Position = DP(2);
        this.WriteString(value);
    }

    public any speak() {
        any __retval;
        this.Position = DP(3);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_Finish(__retval);
        return __retval;
    }

    public any take_damage(int amount) {
        any __retval;
        this.Position = DP(4);
        Function func = this.ReadFunction();
        this.Reset();
        Call_StartFunction(INVALID_HANDLE, func);
        Call_PushCell(this);
        Call_PushCell(amount);
        Call_Finish(__retval);
        return __retval;
    }
}

methodmap Peter < Person {
    public Peter() {
        Person sm_base = new Person();
        Peter sm = view_as<Peter>(sm_base);

        sm.Position = DP(2);
        sm.WriteString("Peter");
        sm.Position = DP(3);
        sm.WriteFunction(Peter_speak);

        return sm;
    }

    public void getOccupation(char[] buffer, int maxlen) {
        this.Position = DP(5);
        this.ReadString(buffer, maxlen);
    }
    public void setOccupation(const char[] value) {
        this.Position = DP(5);
        this.WriteString(value);
    }
}

stock Person Person_new(char[] name, int health) {
    return new Person(name, health);
}

stock Person Peter_new() {
    return new Peter();
}


public any Person_speak(Person _inst) {
    char buffer[256];
    FormatEx(buffer, sizeof(buffer), "Hello, my name is %s", __buf_name); PrintToServer(buffer);
}

public any Person_take_damage(Person _inst, int amount) {
    char buffer[256];
    _inst.health -= amount;
    if ((_inst.health <= 0)) {
        _inst.is_alive = false;
        FormatEx(buffer, sizeof(buffer), "%s has fallen.", __buf_name); PrintToServer(buffer);
    }
}

public any Peter_speak(Peter _inst) {
    char buffer[256];
    FormatEx(buffer, sizeof(buffer), "I'm Peter, and I'm a %s", __buf_occupation); PrintToServer(buffer);
}

