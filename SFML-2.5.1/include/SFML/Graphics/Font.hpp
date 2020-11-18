
#ifndef SFML_FONT_HPP
#define SFML_FONT_HPP


#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Glyph.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/String.hpp>
#include <map>
#include <string>
#include <vector>


namespace sf
{
class InputStream;


class SFML_GRAPHICS_API Font
{
public:

    
    struct Info
    {
        std::string family; 
    };

public:

   
    Font();

    
    Font(const Font& copy);

    
    ~Font();

    
    bool loadFromFile(const std::string& filename);

    
    bool loadFromMemory(const void* data, std::size_t sizeInBytes);

    
    bool loadFromStream(InputStream& stream);

   
    const Info& getInfo() const;

    
    const Glyph& getGlyph(Uint32 codePoint, unsigned int characterSize, bool bold, float outlineThickness = 0) const;

    
    float getKerning(Uint32 first, Uint32 second, unsigned int characterSize) const;

   
    float getLineSpacing(unsigned int characterSize) const;

    
    float getUnderlinePosition(unsigned int characterSize) const;

    
    float getUnderlineThickness(unsigned int characterSize) const;


    const Texture& getTexture(unsigned int characterSize) const;

    
    Font& operator =(const Font& right);

private:

    
    struct Row
    {
        Row(unsigned int rowTop, unsigned int rowHeight) : width(0), top(rowTop), height(rowHeight) {}

        unsigned int width;  
        unsigned int top;    
        unsigned int height; 
    };

    
    typedef std::map<Uint64, Glyph> GlyphTable; 

   
    struct Page
    {
        Page();

        GlyphTable       glyphs;  
        Texture          texture; 
        unsigned int     nextRow;
        std::vector<Row> rows;    
    };

    
    
    void cleanup();

   
    Glyph loadGlyph(Uint32 codePoint, unsigned int characterSize, bool bold, float outlineThickness) const;

    
    IntRect findGlyphRect(Page& page, unsigned int width, unsigned int height) const;

    
    bool setCurrentSize(unsigned int characterSize) const;

    
    typedef std::map<unsigned int, Page> PageTable; 

    
    void*                      m_library;     
    void*                      m_face;        
    void*                      m_streamRec;   
    void*                      m_stroker;     
    int*                       m_refCount;    
    Info                       m_info;       
    mutable PageTable          m_pages;       
    mutable std::vector<Uint8> m_pixelBuffer; 
    #ifdef SFML_SYSTEM_ANDROID
    void*                      m_stream; 
    #endif
};

} 


#endif 



