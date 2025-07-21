from typing import Self



class BasePerk:
    __NAME__ = "Unselected"
    __SHORTDESC__ = "Unselected"
    __DESC__ = "Please select one perk to check info"
    client: int = -1
    def __init__(self, client: int):
        self.client = client

    # Event Handlers
    def onPlayerRunCmd(self, buttons: int, impulse: int, vel: 'list[float]', angles: 'list[float]', weapon: int) -> None:
        """
        在玩家执行指令（如跳跃、攻击）时触发。
        """
        pass

    def onAmmoPickup(self, pickup: int) -> None:
        """
        当玩家拾取弹药包时触发。
        """
        pass

    def onCalcIsAttackCritical(self) -> None:
        """
        在系统计算某次攻击是否为暴击时触发。
        """
        pass

    def onCallForMedic(self) -> None:
        """
        当玩家按下“呼叫医生”键时触发。
        """
        pass

    def onGameFrame(self) -> None:
        """
        在游戏的每一帧（或一个非常短的时间间隔）执行。
        """
        pass

    def onGraceEnd(self) -> None:
        """
        在回合开始的准备时间结束后触发。
        """
        pass

    def onMedPickup(self, pickup: int) -> None:
        """
        当玩家拾取医疗包时触发。
        """
        pass

    def onPeriodic(self) -> None:
        """
        周期性（通常是每秒）为所有玩家执行一次的函数。
        """
        pass

    def onPlayerDeath(self, victim: int, killer: int, assist: int, inflictor: int, damagetype: int) -> None:
        """
        当任何玩家或僵尸死亡时触发。
        """
        pass

    def onPlayerSpawn(self) -> None:
        """
        在玩家重生时触发。
        """
        pass

    def onRemove(self) -> None:
        """
        在玩家取消选择此职业时触发
        """
        pass

    def onSetTransmit(self, entity: int, client: int) -> None:
        """
        在确定一个实体是否应该对特定客户端可见时触发。
        """
        pass

    def onTakeDamage(self, victim: int, attacker: int, inflictor: int, damage: float, damagetype: int) -> None:
        """
        在伤害事件发生时（伤害计算前）触发。
        """
        pass

    def onTakeDamagePost(self, victim: int, attacker: int, inflictor: int, damage: float, damagetype: int) -> None:
        """
        在伤害事件发生后（伤害计算后）触发。
        """
        pass

    def onTouch(self, toucher: int, touchee: int) -> None:
        """
        当一个实体触摸到另一个实体时触发。
        """
        pass

    # Other functions
    def updateClientPermStats(self) -> None:
        """
        更新玩家的永久属性，通常在重生时调用。
        """
        pass

    def updateCondStats(self) -> None:
        """
        周期性（通常每秒）更新玩家的状态、冷却时间和HUD。
        """
        pass

    def doItemThrow(self, model: str, force: float, color: 'tuple[int, int, int]') -> None:
        """
        执行投掷物品的逻辑。
        """
        pass


