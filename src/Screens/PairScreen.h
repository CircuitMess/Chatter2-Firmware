#ifndef CHATTER_FIRMWARE_PAIRSCREEN_H
#define CHATTER_FIRMWARE_PAIRSCREEN_H

#include "../Interface/LVScreen.h"
#include "vector"
#include "../Model/Profile.hpp"
#include "../Services/Pair/PairService.h"
#include "../AutoPop.h"

class PairScreen : public LVScreen {
public:
	PairScreen();
	virtual ~PairScreen();
	void onStart() override;
	void onStop() override;

private:
	lv_obj_t* pairAnimation;
	lv_anim_t scanningAnim;
	lv_obj_t* scanLabel;
	static void threeDotsAnim(void* obj, int32_t v);

	lv_obj_t* singleUserLayout;
	lv_obj_t* pairPrompt;
	lv_obj_t* userList;
	lv_obj_t* waitLayout;

	lv_style_t btnDefault;
	lv_style_t btnFocused;

	uint32_t userCount = 0;
	uint32_t activeIndex = -1;

	void createScanLabel();
	void setupScan();
	void addUserToList(const Friend &user);
	void selectUser(uint32_t index);
	void userFound(const Friend &user);
	void userChanged(const Friend &user, uint32_t index);
	void createSingleUser(const Friend &user);
	void createUserList();

	void createPairPrompt(uint32_t index);
	void confirmPair();
	void refusePair();
	void createWaiting();
	void cancelPair();


	void pairDone();
	void pairFail();

	PairService pair;
	AutoPop autoPop;

};


#endif //CHATTER_FIRMWARE_PAIRSCREEN_H
