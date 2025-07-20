from typing import Any, list, Callable, Union


def FindTeamByName(name: str) -> int:
    """Given a partial team name, attempts to find a matching team.

The search is performed case insensitively and only against the 
first N characters of the team names, where N is the number of 
characters in the search pattern.

@param name          Partial or full team name.  
@return              A valid team index on success.
                     -1 if no team matched.
                     -2 if more than one team matched."""
    pass
name_len: int = ...
num_teams: int = ...
found_team: int = ...