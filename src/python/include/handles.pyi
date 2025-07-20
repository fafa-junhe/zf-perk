from typing import Any, list, Callable, Union


class Handle:
    """Preset Handle values."""
    INVALID_HANDLE: int = ...
    def Clone(self, plugin: Any = ...) -> Any:
        pass
    def Close(self) -> None:
        pass
    def __del__(self) -> None:
        pass


def CloseHandle(hndl: Any) -> None:
    """Closes a Handle.  If the handle has multiple copies open,
it is not destroyed unless all copies are closed.

@note Closing a Handle has a different meaning for each Handle type.  Make
      sure you read the documentation on whatever provided the Handle.

@param hndl      Handle to close.
@error           Invalid handles will cause a run time error."""
    pass
def CloneHandle(hndl: Any, plugin: Any = ...) -> Any:
    """Clones a Handle.  When passing handles in between plugins, caching handles
can result in accidental invalidation when one plugin releases the Handle, or is its owner
is unloaded from memory.  To prevent this, the Handle may be "cloned" with a new owner.

@note Usually, you will be cloning Handles for other plugins.  This means that if you clone
      the Handle without specifying the new owner, it will assume the identity of your original
      calling plugin, which is not very useful.  You should either specify that the receiving
      plugin should clone the handle on its own, or you should explicitly clone the Handle
      using the receiving plugin's identity Handle.

@param hndl      Handle to clone/duplicate.
@param plugin    Optional Handle to another plugin to mark as the new owner.
                 If no owner is passed, the owner becomes the calling plugin.
@return          Handle on success, INVALID_HANDLE if not cloneable.
@error           Invalid handles will cause a run time error."""
    pass
def IsValidHandle(hndl: Any) -> bool:
    pass