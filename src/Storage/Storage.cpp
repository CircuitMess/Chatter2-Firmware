#include "Storage.h"

Repositories Storage = {
		MessageRepo("/Repo/Msg"),
		ConvoRepo("/Repo/Convo"),
		Repo<Friend>("/Repo/Friends")
};