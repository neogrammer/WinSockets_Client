#include <level/LayeredBackground.h>
#include <SFML/Graphics/Sprite.hpp>
#include <core/globals.h>
LayeredBackground::LayeredBackground(sf::Texture& t1, sf::Texture& t2, sf::Texture& t3, sf::Texture& t4, float lag1, float lag2, float lag3, float lag4)
	: bgLayers{}
	, layerVelocityLag{}
	, xPos{}
{
	bgLayers.clear();
	layerVelocityLag.clear();
	xPos.clear();
	bgLayers.reserve(4);
	layerVelocityLag.reserve(4);
	xPos.reserve(4);
	bgLayers.push_back(&t1);
	bgLayers.push_back(&t2);
	bgLayers.push_back(&t3);
	bgLayers.push_back(&t4);
	layerVelocityLag.push_back(lag1);
	layerVelocityLag.push_back(lag2);
	layerVelocityLag.push_back(lag3);
	layerVelocityLag.push_back(lag4);
	xPos.push_back(0.f);
	xPos.push_back(0.f);
	xPos.push_back(0.f);
	xPos.push_back(0.f);

}

LayeredBackground::~LayeredBackground()
{
}

void LayeredBackground::renderForeground()
{
	for (int i = 3; i < 5; i++)
	{
		sf::Sprite spr = sf::Sprite{};
		spr.setTexture(*bgLayers[i]);
		spr.setPosition({ xPos[i], 900.f - spr.getTexture()->getSize().y });

		gWnd.draw(spr);
	}
}
void LayeredBackground::render()
{
	if (!parallax)
	{
		for (int i = 0; i < 4; i++)
		{
			sf::Sprite spr = sf::Sprite{};
			spr.setTexture(*bgLayers[i]);
			spr.setPosition({ xPos[i],0.f });

			gWnd.draw(spr);
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			sf::Sprite spr = sf::Sprite{};
			spr.setTexture(*bgLayers[i]);
			spr.setPosition({ xPos[i], 900.f - spr.getTexture()->getSize().y });

			gWnd.draw(spr);
		}
	}
}
