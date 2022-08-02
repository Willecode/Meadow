#pragma once
#include "resource_management/shadermanager.h"
class PostProcessing
{
public:
	PostProcessing();
	void init(ShaderManager* sdrMan);

	/*
	* Set postprocessing and antialiasing flags
	*/
	void sharpen(bool f);
	void grayscale(bool f);
	void negate(bool f);
	void MSAA(bool f);

	/*
	* Get postprocessing and antialiasing flags
	*/
	bool getSharpen() const;
	bool getGrayscale() const;
	bool getNegate() const;
	bool getMSAA() const;
private:
	ShaderManager* m_sdrMan;
	// These are passed to postproc shader
	bool m_sharpen, m_grayscale, m_negate;

	// This is just a flag that others can get and this class keeps track of.
	bool m_MSAA;

};

