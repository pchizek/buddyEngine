/*
 * Loader.cpp
 * For loading level data from xml files using the tinyxml2 library
 *
 * Written by: Patrick Chizek	https://github.com/pchizek
 * tinyxml2 by: Lee Thomason	https://github.com/leethomason
 */

#include <stdio.h>
#include <iostream>
#include <string>
#include "tinyxml2.h"
#include "loader.h"
#include "..//managers/objectManager2d.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace tinyxml2;

/* Extracts a set of sibling elements from an xml file 
 * 
 * Arguments:
 * firstRef: a pointer to the first node of a linked list that stores the extracted strings
 * parentElement: a pointer to the parent element of the elements that you want to extract from
 * elementName: a char array that is the name of the element you want to extract
 *
 * Returns: number of elements extracted from the xml file
 */
int extractElementSet(node* firstRef, XMLElement* parentElement, const char elementName[]) {

	// Find the first element of the type you want to extract
	XMLElement* extract = parentElement->FirstChildElement(elementName);

	unsigned int numElements = 1;

	node* newNode = firstRef;

	while (extract) {

		/* Extract value */
		string temp = (string)extract->GetText();

		/* Put the data in the node  */
		newNode->data = temp;

		/* Define the new node as the end by making its next a NULL pointer */
		newNode->next = NULL;

		/* 4. If the Linked List is empty, then make the new node as head */
		node* last = firstRef;

		while (last->next != NULL) {
			last = last->next;
		}

		/* Change the next pointer of what was the last node to point to the new last node */
		if (newNode != firstRef) {
			last->next = newNode;
		}

		/* Get the next sibling element of the XML */
		parentElement = parentElement->NextSiblingElement();

		/* If next sibling element is NULL, we're done extracting this set of elements */
		if (!parentElement) {
			return numElements;

		}

		/* If there are more elements, get the element to extract from */
		extract = parentElement->FirstChildElement(elementName);

		/* Allocate a new node for this string */
		newNode = new node;

		numElements++;

	}

	/* If there's nothing to extract, return 0 */
	return 0;
}

/* Parses a string to extract various integer and array values
 *
 * Arguments:
 * inputString: an object of type std::string that values will be extracted from
 *
 * (Overloaded)
 * tupleArray: an array of 2 integers (such as 2D coordinates)
 * num: a single integer
 * 
 *
 * Returns: 0 on success, -1 on error
 */
int parse(string inputString, int tupleArray[2]) {

	size_t length = inputString.length();

	if ((inputString[0] == '[') && (inputString[length - 1] == ']')) {

		//Find index of comma
		if (inputString.find(',', 1) == string::npos) {
			return -1;
		}

		size_t idx = 0;

		for (int i = 0; i < 2; i++) {
			inputString = inputString.substr(idx + 1);
			tupleArray[i] = stoi(inputString, &idx, 10);
		}

	}

	else {
		return -1;
	}

	return 0;
}

int parse(string inputString) {

	size_t length = inputString.length();

	return stoi(inputString, &length, 10);

}

/* Loads an entire level into memory from an xml file
 *
 * Arguments:
 * filename: a char array that is the name of the file to be loaded
 *
 * Returns: 0 on success, error codes
 */
int loadLevel(const char filename[]) {

	/* Declare the struct that holds all the objects */
	extern struct objects levelObjects;

	// Get the window that will display the level
	extern sf::RenderWindow window;

	XMLDocument levelDoc;

		// Open file
		XMLError loadError = levelDoc.LoadFile(filename);

	// Throw error
	if (loadError) {
		return loadError;
	}

	// Define elements
	XMLElement* level = levelDoc.FirstChildElement("level");

	XMLElement* assets = level->FirstChildElement("assets");
	XMLElement* blockset = assets->FirstChildElement("blockset");

	XMLElement* environment = level->FirstChildElement("environment");
	XMLElement* blockElement = environment->FirstChildElement("block");


	/* Level Characteristics */

	// Assemble macro grid
	//string grid = (string)level->FirstChildElement("macroSize")->GetText();

	//int rc = parse(grid, macroGridDims);

	/* Load blockset data from file */
	node	setIDs;
	node	setFileNames;
	node	setNumBlocks;

	int numBlocksets = extractElementSet(&setIDs,
		blockset,
		"setNum");
	extractElementSet(&setFileNames,
		blockset,
		"file");
	extractElementSet(&setNumBlocks,
		blockset,
		"numBlocks");

	/* Create static vector to hold all textures */
	static vector<sf::Texture*> texPtrs(numBlocksets);

	/* Store all blockset textures */
	for (int i = 0; i < numBlocksets; i++) {

		if (i) {
			texPtrs.push_back(new sf::Texture());
		}
		else {
			texPtrs.front() = new sf::Texture();
		}

		string filepath = "resources/blocksets/" + setFileNames.data;
		if (!texPtrs.back()->loadFromFile(filepath)) {
			throw "Texture File could not be opened";
		}
	}

	/* Load blocks data from file */
	node	blockSetNode;
	node	selectionNode;
	node	gridCoordsNode;

	int numBlocks = extractElementSet(&blockSetNode,
		blockElement,
		"set");
	extractElementSet(&selectionNode,
		blockElement,
		"select");
	extractElementSet(&gridCoordsNode,
		blockElement,
		"world");

	/* Initialize all blocks */
	block* newBlock = NULL;
	levelObjects.numBlocks = numBlocks;

	for (int i = 0; i < numBlocks; i++) {
		
		/* Get which blockset this block will use */
		int blockSetNum = parse(blockSetNode.data);
		
		/* Get which block in the set it is */
		int select = parse(selectionNode.data);

		/* Get world grid coordinates of this block */
		int gridCoords[2];
		parse(gridCoordsNode.data,gridCoords);

		/* Create the new block */
		if (!i) {
			newBlock = new block(texPtrs.at(blockSetNum), gridCoords, select, true);
			levelObjects.firstBlock = newBlock;
		}
		else {
			
			newBlock->nextBlock = new block(texPtrs.at(blockSetNum), gridCoords, select, true);
			newBlock = newBlock->nextBlock;
		}

		/* Go to the next object in all the linked lists */
		if (blockSetNode.next){
			blockSetNode = *blockSetNode.next;
			selectionNode = *selectionNode.next;
			gridCoordsNode = *gridCoordsNode.next;
		}

	}

	// Load other images/props

	// Load entities

	// Load player

	return 0;
}
