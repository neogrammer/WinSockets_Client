#ifndef LAYEREDBACKGROUND_H__
#define LAYEREDBACKGROUND_H__

#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

class LayeredBackground
{
	std::vector<sf::Texture*> bgLayers{};
	std::vector<float> layerVelocityLag{};
	std::vector<float> xPos{};
public:
	LayeredBackground();
	LayeredBackground(sf::Texture& t1, sf::Texture& t2, sf::Texture& t3, sf::Texture& t4, float lag1, float lag2, float lag3, float lag4);
	~LayeredBackground();
	LayeredBackground(const LayeredBackground&) = delete;
	LayeredBackground& operator=(const LayeredBackground&) = delete;
	LayeredBackground(LayeredBackground&&) = delete;
	LayeredBackground& operator=(LayeredBackground&&) = delete;


	void render();

};
#endif