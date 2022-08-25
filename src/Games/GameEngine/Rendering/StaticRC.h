#ifndef CIRCUITPET_FIRMWARE_STATICRC_H
#define CIRCUITPET_FIRMWARE_STATICRC_H

#include "RenderComponent.h"

class StaticRC : public RenderComponent{
public:
	/**
	 * @param file Raw RGB565 image file.
	 * @param dim Width, height.
	 */
	StaticRC(File file, PixelDim dim);
	~StaticRC() override = default;

	void setFile(File file); //no change in dim
	void setFile(File file, PixelDim dim); //change in dim

protected:
	void push(Sprite* parent, PixelDim pos, float rot) const override;

private:
	File file;
	PixelDim dim;
};


#endif //CIRCUITPET_FIRMWARE_STATICRC_H
