#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <string>
#include <vector>
#include <utility>
class TextRenderer {
	
	class TextInstance {
		std::string str;
		int xPos;
		int yPos;
	};

	std::vector<TextInstance> textList;
public:
	void drawText(std::string text, int xPos, int yPos);

	//Renders all text in the buffers
	//Clears the buffer after render
	void renderText();
};




#endif