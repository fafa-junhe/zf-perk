from typing import Any, list, Callable, Union
from .core import *


"""Called when an entity output is fired.

@param output        Name of the output that fired.
@param caller        Entity index of the caller.
@param activator     Entity index of the activator.
@param delay         Delay in seconds? before the event gets fired.
@return              Anything other than Plugin_Continue will supress this event,
                     returning Plugin_Continue will allow it to propagate the results
                     of this output to any entity inputs."""
EntityOutput = Union[
    Callable[[str, int, int, float], None],
    Callable[[str, int, int, float], Any]
]
def HookEntityOutput(classname: str, output: str, callback: EntityOutput) -> None:
    """Add an entity output hook on a entity classname

@param classname     The classname to hook.
@param output        The output name to hook.
@param callback      An EntityOutput function pointer.
@error               Entity Outputs disabled."""
    pass
def UnhookEntityOutput(classname: str, output: str, callback: EntityOutput) -> bool:
    """Remove an entity output hook.
@param classname     The classname to hook.
@param output        The output name to hook.
@param callback      An EntityOutput function pointer.
@return              True on success, false if no valid hook was found.
@error               Entity Outputs disabled."""
    pass
def HookSingleEntityOutput(entity: int, output: str, callback: EntityOutput, once: bool = ...) -> None:
    """Add an entity output hook on a single entity instance

@param entity        The entity on which to add a hook.
@param output        The output name to hook.
@param callback      An EntityOutput function pointer.
@param once          Only fire this hook once and then remove itself.
@error               Entity Outputs disabled or Invalid Entity index."""
    pass
def UnhookSingleEntityOutput(entity: int, output: str, callback: EntityOutput) -> bool:
    """Remove a single entity output hook.

@param entity        The entity on which to remove the hook.
@param output        The output name to hook.
@param callback      An EntityOutput function pointer.
@return              True on success, false if no valid hook was found.
@error               Entity Outputs disabled or Invalid Entity index."""
    pass
def FireEntityOutput(caller: int, output: str, activator: int = ..., delay: float = ...) -> None:
    """Fire a named output on an entity.

After completion (successful or not), the current global variant is re-initialized.

@param caller        Entity index from where the output is fired.
@param output        Output name.
@param activator     Entity index which initiated the sequence of actions (-1 for a NULL entity).
@param delay         Delay before firing the output.
@error               Invalid entity index or no mod support."""
    pass