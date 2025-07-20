from typing import Any, list, Callable, Union


SNDCHAN_REPLACE: int = ...
SNDCHAN_AUTO: int = ...
SNDCHAN_WEAPON: int = ...
SNDCHAN_VOICE: int = ...
SNDCHAN_ITEM: int = ...
SNDCHAN_BODY: int = ...
SNDCHAN_STREAM: int = ...
SNDCHAN_STATIC: int = ...
SNDCHAN_VOICE_BASE: int = ...
SNDCHAN_USER_BASE: int = ...
SND_NOFLAGS: int = ...
SND_CHANGEVOL: int = ...
SND_CHANGEPITCH: int = ...
SND_STOP: int = ...
SND_SPAWNING: int = ...
SND_DELAY: int = ...
SND_STOPLOOPING: int = ...
SND_SPEAKER: int = ...
SND_SHOULDPAUSE: int = ...
SND_IGNORE_PHONEMES: int = ...
SND_IGNORE_NAME: int = ...
SND_DO_NOT_OVERWRITE_EXISTING_ON_CHANNEL: int = ...
SNDLEVEL_NONE: int = ...
SNDLEVEL_RUSTLE: int = ...
SNDLEVEL_WHISPER: int = ...
SNDLEVEL_LIBRARY: int = ...
SNDLEVEL_FRIDGE: int = ...
SNDLEVEL_HOME: int = ...
SNDLEVEL_CONVO: int = ...
SNDLEVEL_DRYER: int = ...
SNDLEVEL_DISHWASHER: int = ...
SNDLEVEL_CAR: int = ...
SNDLEVEL_NORMAL: int = ...
SNDLEVEL_TRAFFIC: int = ...
SNDLEVEL_MINIBIKE: int = ...
SNDLEVEL_SCREAMING: int = ...
SNDLEVEL_TRAIN: int = ...
SNDLEVEL_HELICOPTER: int = ...
SNDLEVEL_SNOWMOBILE: int = ...
SNDLEVEL_AIRCRAFT: int = ...
SNDLEVEL_RAIDSIREN: int = ...
SNDLEVEL_GUNFIRE: int = ...
SNDLEVEL_ROCKET: int = ...
def PrefetchSound(name: str) -> None:
    """Prefetches a sound.

@param name          Sound file name relative to the "sound" folder."""
    pass
def GetSoundDuration(name: str) -> float:
    pass
def EmitAmbientSound(name: str, pos: list[float], entity: int = ..., level: int = ..., flags: int = ..., vol: float = ..., pitch: int = ..., delay: float = ...) -> None:
    """Emits an ambient sound.

@param name          Sound file name relative to the "sound" folder.
@param pos           Origin of sound.
@param entity        Entity index to associate sound with.
@param level         Sound level (from 0 to 255).
@param flags         Sound flags.
@param vol           Volume (from 0.0 to 1.0).
@param pitch         Pitch (from 0 to 255).
@param delay         Play delay."""
    pass
def FadeClientVolume(client: int, percent: float, outtime: float, holdtime: float, intime: float) -> None:
    """Fades a client's volume level toward silence or a given percentage.

@param client        Client index.
@param percent       Fade percentage.
@param outtime       Fade out time, in seconds.
@param holdtime      Hold time, in seconds.
@param intime        Fade in time, in seconds.
@error               Invalid client index or client not in game."""
    pass
def StopSound(entity: int, channel: int, name: str) -> None:
    """Stops a sound.

@param entity        Entity index.
@param channel       Channel number.
@param name          Sound file name relative to the "sound" folder."""
    pass
def EmitSound(clients: list[int], numClients: int, sample: str, entity: int = ..., channel: int = ..., level: int = ..., flags: int = ..., volume: float = ..., pitch: int = ..., speakerentity: int = ..., origin: list[float] = ..., dir: list[float] = ..., updatePos: bool = ..., soundtime: float = ..., _0: Any = ..., *args: Any) -> None:
    """Emits a sound to a list of clients.

@param clients       Array of client indexes.
@param numClients    Number of clients in the array.
@param sample        Sound file name relative to the "sound" folder.
@param entity        Entity to emit from.
@param channel       Channel to emit with.
@param level         Sound level.
@param flags         Sound flags.
@param volume        Sound volume.
@param pitch         Sound pitch.
@param speakerentity Unknown.
@param origin        Sound origin.
@param dir           Sound direction.
@param updatePos     Unknown (updates positions?)
@param soundtime     Alternate time to play sound for.
@param ...           Optional list of Float[3] arrays to specify additional origins.
@error               Invalid client index or client not in game."""
    pass
def EmitSoundEntry(clients: list[int], numClients: int, soundEntry: str, sample: str, entity: int = ..., channel: int = ..., level: int = ..., seed: int = ..., flags: int = ..., volume: float = ..., pitch: int = ..., speakerentity: int = ..., origin: list[float] = ..., dir: list[float] = ..., updatePos: bool = ..., soundtime: float = ..., _0: Any = ..., *args: Any) -> None:
    """Emits a sound or game sound to a list of clients using the latest version of the engine sound interface.
This native is only available in engines that are greater than or equal to Portal 2.

@param clients       Array of client indexes.
@param numClients    Number of clients in the array.
@param soundEntry    Sound entry name.
@param sample        Sound file name relative to the "sound" folder.
@param entity        Entity to emit from.
@param channel       Channel to emit with.
@param level         Sound level.
@param seed          Sound seed.
@param flags         Sound flags.
@param volume        Sound volume.
@param pitch         Sound pitch.
@param speakerentity Unknown.
@param origin        Sound origin.
@param dir           Sound direction.
@param updatePos     Unknown (updates positions?)
@param soundtime     Alternate time to play sound for.
@param ...           Optional list of Float[3] arrays to specify additional origins.
@error               Invalid client index, client not in game, or lack of mod support."""
    pass
def EmitSentence(clients: list[int], numClients: int, sentence: int, entity: int, channel: int = ..., level: int = ..., flags: int = ..., volume: float = ..., pitch: int = ..., speakerentity: int = ..., origin: list[float] = ..., dir: list[float] = ..., updatePos: bool = ..., soundtime: float = ..., _0: Any = ..., *args: Any) -> None:
    """Emits a sentence to a list of clients.

@param clients       Array of client indexes.
@param numClients    Number of clients in the array.
@param sentence      Sentence index (from PrecacheSentenceFile).
@param entity        Entity to emit from.
@param channel       Channel to emit with.
@param level         Sound level.
@param flags         Sound flags.
@param volume        Sound volume.
@param pitch         Sound pitch.
@param speakerentity Unknown.
@param origin        Sound origin.
@param dir           Sound direction.
@param updatePos     Unknown (updates positions?)
@param soundtime     Alternate time to play sound for.
@param ...           Optional list of Float[3] arrays to specify additional origins.
@error               Invalid client index or client not in game."""
    pass
def GetDistGainFromSoundLevel(soundlevel: int, distance: float) -> float:
    """Calculates gain of sound on given distance with given sound level in decibel

@param soundlevel    decibel of sound, like SNDLEVEL_NORMAL or integer value
@param distance      distance of sound to calculate, not meter or feet, but Source Engine`s normal Coordinate unit
@return              gain of sound. you can multiply this with original sound`s volume to calculate volume on given distance"""
    pass
def AddAmbientSoundHook(hook: Any) -> None:
    """Hooks all played ambient sounds.

@param hook          Function to use as a hook.
@error               Invalid function hook."""
    pass
def AddNormalSoundHook(hook: NormalSHook) -> None:
    """Hooks all played normal sounds.

@param hook          Function to use as a hook.
@error               Invalid function hook."""
    pass
def RemoveAmbientSoundHook(hook: Any) -> None:
    """Unhooks all played ambient sounds.

@param hook          Function used for the hook.
@error               Invalid function hook."""
    pass
def RemoveNormalSoundHook(hook: NormalSHook) -> None:
    """Unhooks all played normal sounds.

@param hook          Function used for the hook.
@error               Invalid function hook."""
    pass
def EmitSoundToClient(client: int, sample: str, entity: int = ..., channel: int = ..., level: int = ..., flags: int = ..., volume: float = ..., pitch: int = ..., speakerentity: int = ..., origin: list[float] = ..., dir: list[float] = ..., updatePos: bool = ..., soundtime: float = ...) -> None:
    """Wrapper to emit sound to one client.

@param client        Client index.
@param sample        Sound file name relative to the "sound" folder.
@param entity        Entity to emit from.
@param channel       Channel to emit with.
@param level         Sound level.
@param flags         Sound flags.
@param volume        Sound volume.
@param pitch         Sound pitch.
@param speakerentity Unknown.
@param origin        Sound origin.
@param dir           Sound direction.
@param updatePos     Unknown (updates positions?)
@param soundtime     Alternate time to play sound for.
@error               Invalid client index or client not in game."""
    pass
def EmitSoundToAll(sample: str, entity: int = ..., channel: int = ..., level: int = ..., flags: int = ..., volume: float = ..., pitch: int = ..., speakerentity: int = ..., origin: list[float] = ..., dir: list[float] = ..., updatePos: bool = ..., soundtime: float = ...) -> None:
    """Wrapper to emit sound to all clients.

@param sample        Sound file name relative to the "sound" folder.
@param entity        Entity to emit from.
@param channel       Channel to emit with.
@param level         Sound level.
@param flags         Sound flags.
@param volume        Sound volume.
@param pitch         Sound pitch.
@param speakerentity Unknown.
@param origin        Sound origin.
@param dir           Sound direction.
@param updatePos     Unknown (updates positions?)
@param soundtime     Alternate time to play sound for.
@error               Invalid client index."""
    pass
def ATTN_TO_SNDLEVEL(attn: float) -> int:
    """Converts an attenuation value to a sound level.
This function is from the HL2SDK.

@param attn          Attenuation value.
@return              Integer sound level."""
    pass
def GetGameSoundParams(gameSound: str, channel: int, soundLevel: int, volume: float, pitch: int, sample: str, maxlength: int, entity: int = ...) -> bool:
    """Retrieves the parameters for a game sound.

Game sounds are found in a game's scripts/game_sound.txt or other files
referenced from it

Note that if a game sound has a rndwave section, one of them will be returned
at random.

@param gameSound     Name of game sound.
@param channel       Channel to emit with.
@param level         Sound level.
@param volume        Sound volume.
@param pitch         Sound pitch.
@param sample        Sound file name relative to the "sound" folder.
@param maxlength     Maximum length of sample string buffer.
@param entity        Entity the sound is being emitted from.
@return              True if the sound was successfully retrieved, false if it
                     was not found"""
    pass
def EmitGameSound(clients: list[int], numClients: int, gameSound: str, entity: int = ..., flags: int = ..., speakerentity: int = ..., origin: list[float] = ..., dir: list[float] = ..., updatePos: bool = ..., soundtime: float = ...) -> bool:
    """Emits a game sound to a list of clients.

Game sounds are found in a game's scripts/game_sound.txt or other files
referenced from it

Note that if a game sound has a rndwave section, one of them will be returned
at random.

@param clients       Array of client indexes.
@param numClients    Number of clients in the array.
@param gameSound     Name of game sound.
@param entity        Entity to emit from.
@param flags         Sound flags.
@param speakerentity Unknown.
@param origin        Sound origin.
@param dir           Sound direction.
@param updatePos     Unknown (updates positions?)
@param soundtime     Alternate time to play sound for.
@return              True if the sound was played successfully, false if it failed
@error               Invalid client index or client not in game."""
    pass
def EmitAmbientGameSound(gameSound: str, pos: list[float], entity: int = ..., flags: int = ..., delay: float = ...) -> bool:
    """Emits an ambient game sound.

Game sounds are found in a game's scripts/game_sound.txt or other files
referenced from it

Note that if a game sound has a rndwave section, one of them will be returned
at random.

@param gameSound     Name of game sound.
@param pos           Origin of sound.
@param entity        Entity index to associate sound with.
@param flags         Sound flags.
@param delay         Play delay."""
    pass
def EmitGameSoundToClient(client: int, gameSound: str, entity: int = ..., flags: int = ..., speakerentity: int = ..., origin: list[float] = ..., dir: list[float] = ..., updatePos: bool = ..., soundtime: float = ...) -> bool:
    """Wrapper to emit a game sound to one client.

Game sounds are found in a game's scripts/game_sound.txt or other files
referenced from it

Note that if a game sound has a rndwave section, one of them will be returned
at random.

@param client        Client index.
@param gameSound     Name of game sound.
@param entity        Entity to emit from.
@param flags         Sound flags.
@param speakerentity Unknown.
@param origin        Sound origin.
@param dir           Sound direction.
@param updatePos     Unknown (updates positions?)
@param soundtime     Alternate time to play sound for.
@error               Invalid client index or client not in game."""
    pass
def EmitGameSoundToAll(gameSound: str, entity: int = ..., flags: int = ..., speakerentity: int = ..., origin: list[float] = ..., dir: list[float] = ..., updatePos: bool = ..., soundtime: float = ...) -> bool:
    """Wrapper to emit game sound to all clients.

Game sounds are found in a game's scripts/game_sound.txt or other files
referenced from it

Note that if a game sound has a rndwave section, one of them will be returned
at random.

@param gameSound     Name of game sound.
@param entity        Entity to emit from.
@param flags         Sound flags.
@param speakerentity Unknown.
@param origin        Sound origin.
@param dir           Sound direction.
@param updatePos     Unknown (updates positions?)
@param soundtime     Alternate time to play sound for.
@error               Invalid client index."""
    pass
def PrecacheScriptSound(soundname: str) -> bool:
    """Precache a game sound.

Most games will precache all game sounds on map start, but this is not guaranteed...
Team Fortress 2 is known to not pre-cache MvM game mode sounds on non-MvM maps.

Due to the above, this native should be called before any calls to GetGameSoundParams,
EmitGameSound*, or EmitAmbientGameSound.

It should be safe to pass already precached game sounds to this function.

Note: It precaches all files for a game sound.

@param soundname     Game sound to precache
@return              True if the game sound was found, false if sound did not exist
                     or had no files"""
    pass
SOUND_FROM_PLAYER: Any = ...  # -2
SOUND_FROM_LOCAL_PLAYER: Any = ...  # -1
SOUND_FROM_WORLD: Any = ...  # 0
SNDVOL_NORMAL: Any = ...  # 1.0     /**< Normal volume */
SNDPITCH_NORMAL: Any = ...  # 100     /**< Normal pitch */
SNDPITCH_LOW: Any = ...  # 95      /**< A low pitch */
SNDPITCH_HIGH: Any = ...  # 120     /**< A high pitch */
SNDATTN_NONE: Any = ...  # 0.0     /**< No attenuation */
SNDATTN_NORMAL: Any = ...  # 0.8     /**< Normal attenuation */
SNDATTN_STATIC: Any = ...  # 1.25    /**< Static attenuation? */
SNDATTN_RICOCHET: Any = ...  # 1.5     /**< Ricochet effect */
SNDATTN_IDLE: Any = ...  # 2.0     /**< Idle attenuation? */
delay: float = ...
total: int = ...
entity: int = ...