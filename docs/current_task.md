# 技能僵尸要塞 - 任务状态 - 2025-07-18 17:00
## 用户需求
 @/zf_perk.inc 
开始一个新任务，将zf_perk内的perk拆出来，放到perks/zom_xxx.inc or perks/sur_xxx.inc下，使用类似于
methodmap Dog < StringMap {
    public Dog(int age) {
StringMap sm = new StringMap();
        sm.SetString("name", "Dog"); // init value
        sm.SetValue("age", age); // pass value
        return view_as<Dog>(sm); // We'll create a StringMap, but call it a dog.
    }
    
    public void SetName(const char[] name) {
        this.SetString("name", name);
    }
    
    public void GetName(char[] buffer, int maxlen) {
        this.GetString("name", buffer, maxlen);
    }
    
    public void SetAge(int age) {
        this.SetValue("age", age);
    }
    
    public int GetAge() {
        int age;
        this.GetValue("age", age);
        return age;
    }
    
    public void SetSex(char sex) {
        this.SetValue("sex", sex);
    }
    
    public char GetSex() {
        char sex;
        this.GetValue("sex", sex);
        return sex;
    }
} 
Dog dog = new Dog();
dog.SetName("Jack");
dog.SetAge(13);
dog.SetSex('M');


// use dog somehow, get the values back, do whatever
delete dog; // CLOSE THE HANDLE 
使用以上的这种方式来写出类似于类的效果。写一个总的base_perk，其他perk继承这个baseperk，其中需要包含函数：
1. 事件处理函数： onSpawn onTakeDamage onCallForMedic等
2. 更新函数：updateCondStats updateTempStats updateConds
3. 获取数据函数: getAttack getCrit getRof getSpeed getDefend等
## 当前任务 (Current Task)
> 修改主文件 `zf_perk.inc` 以集成新的Perk对象，包括引入新文件、移除旧定义、创建Perk对象管理器，并重写核心事件处理器以调用新对象的方法。

## 下一个任务 (Next Task)
> 在确认新的Perk对象模型可以正常工作后，开始逐一重构所有剩余的幸存者和僵尸Perks。

## 任务清单 (To-Do List)
- [x] 确认项目结构和重构策略
- [x] 创建 `perks` 目录
- [x] 创建 `perks/base_perk.inc`
- [x] 重构幸存者Perk "运动员" (Athletic)
- [x] 重构僵尸Perk "零号僵尸" (Alpha)
- [-] 修改主文件 `zf_perk.inc` 以集成新的Perk对象
- [ ] (后续) 重构所有剩余的Perks
- [ ] (后续) 清理 `zf_perk.inc` 中的遗留代码

## 本次会话总结 (Session Summary)
> 本次会话中，我们成功地为技能僵尸要塞插件的Perk系统设计并实施了新的面向对象架构。我们创建了`base_perk`基类和`perk_structs`数据文件，并完成了两个Perk（一个幸存者，一个僵尸）作为概念验证。目前正准备开始修改主插件文件以集成这个新系统。

## 注意事项
> 1.  `zom_alpha.inc` 中的 `doAlphaSummon` 和 `onPeriodicUpdate` 的部分逻辑依赖于全局玩家列表，这部分需要在主文件中实现，并通过某种方式（例如函数参数）将上下文传递给Perk对象。
> 2.  下一步修改 `zf_perk.inc` 是一个大手术，需要非常小心，确保所有旧的逻辑都被新的对象调用正确替换。