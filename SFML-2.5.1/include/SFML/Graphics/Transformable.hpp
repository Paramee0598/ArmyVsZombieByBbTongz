#ifndef SFML_TRANSFORMABLE_HPP
#define SFML_TRANSFORMABLE_HPP

#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Transform.hpp>


namespace sf
{

class SFML_GRAPHICS_API Transformable
{
public:
        
    Transformable();
        
    virtual ~Transformable();
        
    void setPosition(float x, float y);//เคลื่อนย้าย

    ////////////////////////////////////////////////////////////
    ///ตำแหน่ง
    ////////////////////////////////////////////////////////////
    void setPosition(const Vector2f& position);//หมุน

    ////////////////////////////////////////////////////////////
    void setRotation(float angle);//องศา
    ////////////////////////////////////////////////////////////
        
    void setScale(float factorX, float factorY);

    ////////////////////////////////////////////////////////////
  
    ////////////////////////////////////////////////////////////
    void setScale(const Vector2f& factors);

    void setOrigin(float x, float y);

    ////////////////////////////////////////////////////////////
    /// 
    ////////////////////////////////////////////////////////////
    void setOrigin(const Vector2f& origin);

    //

    const Vector2f& getPosition() const;

    ////////////////////////////////////////////////////////////
    //องศา
    ////////////////////////////////////////////////////////////
    float getRotation() const;
     
    const Vector2f& getScale() const;

    ////////////////////////////////////////////////////////////
       
    //////////////////////////////////////////////////////////

    const Vector2f& getOrigin() const;

    void move(float offsetX, float offsetY);

    ////////////////////////////////////////////////////////////
    //เคลื่อนย้าย
    ////////////////////////////////////////////////////////////
    void move(const Vector2f& offset);

    
    void rotate(float angle);
    //องศา
    
    void scale(float factorX, float factorY);
        
    void scale(const Vector2f& factor);

    const Transform& getTransform() const;
            
    const Transform& getInverseTransform() const;

private:

    ////////////////////////////////////////////////////////////
    // DATA
    ////////////////////////////////////////////////////////////
    Vector2f          m_origin;                     ///< หมุนปรับขนาดที่ไหน
    Vector2f          m_position;                   ///< ตำแหน่ง
    float             m_rotation;                   ///< องศา
    Vector2f          m_scale;                      
    mutable Transform m_transform;                  ///< เดิม
    mutable bool      m_transformNeedUpdate;        ///< อัพใหม่
    mutable Transform m_inverseTransform;           
    mutable bool      m_inverseTransformNeedUpdate; 
};

} 

#endif 



