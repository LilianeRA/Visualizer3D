#ifndef GOCAD_READER_H
#define GOCAD_READER_H

#include <string>
#include <vector>
//#include <DrawableLines.h>
#include <DrawableSpheres.h>
#include <DrawableTriangles.h>

class GocadReader
{
    public:
        GocadReader();
        virtual ~GocadReader();
        
		void ReadDebugLogger(const std::string &filepath);
		DrawableLines* GetSkeletonLines();
		DrawableLines* GetGridLines();
		DrawableLines* GetCellLines();
		DrawableSpheres* GetCellSpheres();
		DrawableTriangles* GetEnvelopeTriangles();
		//DrawableTriangles* GetGridTriangles();
    protected:

    private:
		DrawableLines *aGridLines;
    	DrawableLines *aSkeletonLines;
    	DrawableLines *aCellLines;
    	DrawableSpheres *aCellSpheres;
		DrawableTriangles *aEnvelopeTri;
		//DrawableTriangles *aGridTri;
        void m_Tokenize(const std::string &text, const std::string &delimiters, std::vector<std::string> &tokenized);
        
};

#endif // GOCAD_READER_H
