#ifndef GOCAD_READER_H
#define GOCAD_READER_H

#include <string>
#include <vector>
/*#include <DrawableLines.h>
#include <DrawableSpheres.h>
#include <DrawableTriangles.h>*/

class GocadReader
{
    public:
        GocadReader();
        virtual ~GocadReader();
        
		void ReadDebugLogger(const std::string &filepath);
		/*DrawableLines* GetGridLines();
		DrawableLines* GetSkeletonLines();
		DrawableLines* GetCellLines();
		DrawableSpheres* GetCellSpheres();
		DrawableTriangles* GetEnvelopetriangles();*/
    protected:

    private:
    	/*DrawableLines *aGridLines;
    	DrawableLines *aSkeletonLines;
    	DrawableLines *aCellLines;
    	DrawableSpheres *aCellSpheres;
		DrawableTriangles *aEnvelopeTri;*/
        void m_Tokenize(const std::string &text, const std::string &delimiters, std::vector<std::string> &tokenized);
        
};

#endif // GOCAD_READER_H
