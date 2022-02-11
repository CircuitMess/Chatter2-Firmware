#ifndef CHATTER_FIRMWARE_SPIFFSCHECKSUM_H
#define CHATTER_FIRMWARE_SPIFFSCHECKSUM_H

#include <Arduino.h>

struct {
	String name;
	uint32_t sum;
} static const SPIFFSChecksums[] PROGMEM = {
		{ "/arrowDown.bin", 2327 },
		{ "/arrowUp.bin", 2327 },
		{ "/bg.bin", 3410322 },
		{ "/colorPicker.bin", 97109 },
		{ "/intro.gif", 11366573 },
		{ "/pairing.gif", 460315 },
		{ "/splash.bin", 75072 },
		{ "/startup.gif", 11380342 },
		{ "/test.bin", 887042 },
		{ "/Unread.bin", 1228 },
		{ "/UserHW.bin", 124494 },

		{ "/Pics/0.bin", 710204 },
        { "/Pics/1.bin", 1098562 },
        { "/Pics/2.bin", 1417767 },
        { "/Pics/3.bin", 768583 },
        { "/Pics/4.bin", 1867123 },
        { "/Pics/5.bin", 1606757 },
        { "/Pics/6.bin", 999570 },
        { "/Pics/7.bin", 795457 },


		{ "/Menu/ArrowDown.bin", 1788 },
		{ "/Menu/ArrowUp.bin", 1789 },

		{ "/Menu/Small/Friends.bin", 1197 },
		{ "/Menu/Small/Inbox.bin", 1180 },
		{ "/Menu/Small/Profile.bin", 1177 },
		{ "/Menu/Small/Settings.bin", 1193 },

		{ "/Menu/Label/Friends.bin", 3048 },
		{ "/Menu/Label/Inbox.bin", 2720 },
		{ "/Menu/Label/Profile.bin", 2954 },
		{ "/Menu/Label/Settings.bin", 3177 },

		{ "/Menu/Big/Friends.gif", 1285866 },
		{ "/Menu/Big/Inbox.gif", 629318 },
		{ "/Menu/Big/Profile.gif", 1106113 },
		{ "/Menu/Big/Settings.gif", 225101 },


		{ "/Emojis/1.bin", 3431 },
		{ "/Emojis/2.bin", 3632 },
		{ "/Emojis/3.bin", 3609 },
		{ "/Emojis/4.bin", 2277 },
		{ "/Emojis/5.bin", 3623 },
		{ "/Emojis/6.bin", 3554 },
		{ "/Emojis/7.bin", 5241 },
		{ "/Emojis/8.bin", 5928 },
		{ "/Emojis/9.bin", 4618 },
		{ "/Emojis/10.bin", 9300 },
		{ "/Emojis/11.bin", 3706 },
		{ "/Emojis/12.bin", 3985 },
		{ "/Emojis/13.bin", 6274 },
		{ "/Emojis/14.bin", 6218 },
		{ "/Emojis/15.bin", 2210 },
		{ "/Emojis/16.bin", 6052 },
		{ "/Emojis/17.bin", 4979 },
		{ "/Emojis/18.bin", 2211 },
		{ "/Emojis/19.bin", 2606 },
		{ "/Emojis/20.bin", 6539 },
		{ "/Emojis/21.bin", 2399 },
		{ "/Emojis/22.bin", 5330 },
		{ "/Emojis/23.bin", 2854 },
		{ "/Emojis/24.bin", 5083 },

		{ "/Battery/0.bin", 2504 },
		{ "/Battery/1.bin", 3699 },
		{ "/Battery/2.bin", 3711 },
		{ "/Battery/3.bin", 3723 },
		{ "/Battery/4.bin", 3735 },
		{ "/Battery/5.bin", 2452 },

		{ "/Avatars/small/1.bin", 5154 },
		{ "/Avatars/small/2.bin", 7125 },
		{ "/Avatars/small/3.bin", 6633 },
		{ "/Avatars/small/4.bin", 6070 },
		{ "/Avatars/small/5.bin", 5906 },
		{ "/Avatars/small/6.bin", 5862 },
		{ "/Avatars/small/7.bin", 5964 },
		{ "/Avatars/small/8.bin", 7451 },
		{ "/Avatars/small/9.bin", 5303 },
		{ "/Avatars/small/10.bin", 5138 },
		{ "/Avatars/small/11.bin", 5971 },
		{ "/Avatars/small/12.bin", 10241 },
		{ "/Avatars/small/13.bin", 6380 },
		{ "/Avatars/small/14.bin", 5682 },
		{ "/Avatars/small/15.bin", 7433 },

		{ "/Avatars/large/1.bin", 12131 },
		{ "/Avatars/large/2.bin", 16609 },
		{ "/Avatars/large/3.bin", 19148 },
		{ "/Avatars/large/4.bin", 18031 },
		{ "/Avatars/large/5.bin", 13055 },
		{ "/Avatars/large/6.bin", 11257 },
		{ "/Avatars/large/7.bin", 14308 },
		{ "/Avatars/large/8.bin", 17415 },
		{ "/Avatars/large/9.bin", 16007 },
		{ "/Avatars/large/10.bin", 18712 },
		{ "/Avatars/large/11.bin", 20315 },
		{ "/Avatars/large/12.bin", 20710 },
		{ "/Avatars/large/13.bin", 14982 },
		{ "/Avatars/large/14.bin", 14100 },
		{ "/Avatars/large/15.bin", 18613 }

};

#endif //CHATTER_FIRMWARE_SPIFFSCHECKSUM_H
