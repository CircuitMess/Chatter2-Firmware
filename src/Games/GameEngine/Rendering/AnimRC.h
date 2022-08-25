#ifndef CIRCUITPET_FIRMWARE_ANIMRC_H
#define CIRCUITPET_FIRMWARE_ANIMRC_H

#include "RenderComponent.h"
#include <Display/GIFAnimatedSprite.h>
#include <functional>

class AnimRC : public RenderComponent {
public:
	/**
	 * @param file GIF file.
	 */
	AnimRC(File file);
	~AnimRC() override = default;
	/**
	 * Replaces the current animation with the one provided.
	 * Will auto-play the anim if the last one was running when the new one was set.
	 * @param file GIF file.
	 */
	void setAnim(File file);

	/**
	 * Sets a function to be executed when the animation loops back again.
	 * To clear the function pass a nullptr or empty parentheses {} as the argument.
	 * The function will be cleared if a new anim is set using setAnim().
	 * @param cb Function to be executed
	 */
	void setLoopDoneCallback(std::function<void(uint32_t)> cb);

	void setLoopMode(GIF::LoopMode loopMode);

	void start();
	void stop();
	void reset();

protected:
	void push(Sprite* parent, PixelDim pos, float rot) const override;

private:
	std::unique_ptr<GIFAnimatedSprite> gif;
	bool playing = false;

	GIF::LoopMode loopMode = GIF::Infinite;
};


#endif //CIRCUITPET_FIRMWARE_ANIMRC_H
