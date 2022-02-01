#include "Storage.h"

Repositories Storage = {
		MessageRepo("/Repo/Msg"),
		ConvoRepo("/Repo/Convo"),
		Repo<Friend>("/Repo/Friends")
};

void Repositories::begin(){
	Messages.begin();
	Convos.begin(true);
	Friends.begin(true);
}
