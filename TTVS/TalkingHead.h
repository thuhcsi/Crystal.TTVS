
#ifndef _CST_TALKING_HEAD_H_
#define _CST_TALKING_HEAD_H_


#ifndef CSTHEAD_EXPORTS
#   define CSTHEAD_API __declspec(dllimport)
#   ifdef _DEBUG
#       pragma comment(lib, "CSTHead.lib")
#       pragma message("        Linking with CSTHead.dll (Debug version)")
#   else
#       pragma comment(lib, "CSTHead.lib")
#       pragma message("        Linking with CSTHead.dll (Release version)")
#   endif
#else
#   define CSTHEAD_API __declspec(dllexport)
#endif


//////////////////////////////////////////////////////////////////////////
//
//  Talking Head related type definition
//
//////////////////////////////////////////////////////////////////////////

///
/// @brief  Camera mode of the Talking Head (for mouse gesture)
///
enum CMODE{ZOOM=1, ROTATE, PAN};

///
/// @brief  Emotion of the Talking Head
///
enum ETYPE{NEUTRAL=0, JOY=1, SADNESS, ANGER, FEAR, DISGUST,SURPRISE};


///
/// @brief  The class for Talking Head manipulation (Interface definition)
///
class CSTHEAD_API TalkingHead
{
public:
    //////////////////////////////////////////////////////////////////////////
    //
    //  Talking Head initialization related operations
    //
    //////////////////////////////////////////////////////////////////////////

    ///
    /// @brief  Constructor
    ///
    /// @param  [in] hWnd   Handle (HWND) of the displaying window.
    ///                     NULL for off-screen rendering.
    ///
    TalkingHead(void* hWnd) : m_bLoaded(false), m_CameraMode(ZOOM) {}

    ///
    /// @brief  Destructor
    ///
    virtual ~TalkingHead() {}

    ///
    /// @brief  Whether Talking Head model is loaded
    ///
    virtual bool isLoaded() const {return m_bLoaded;}

    ///
    /// @brief  Load the Talking Head model
    ///
    /// @param  [in] fileName   Name of the model file
    /// @param  [in] filePath   Path where model files are located
    ///
    virtual bool loadModel(const wchar_t *fileName, const wchar_t *filePath) = 0;

    ///
    /// @brief  Unload the Talking Head model
    ///
    virtual bool unloadModel() = 0;

    ///
    /// @brief  Rebind the textures (for 3D->OpenGL only)
    /// @todo   Remove this interface?
    ///
    virtual bool rebindTextures() {return false;}


public:
    //////////////////////////////////////////////////////////////////////////
    //
    //  Talking Head rendering related operations
    //
    //////////////////////////////////////////////////////////////////////////

    ///
    /// @brief  Render the Talking Head on the displaying window
    ///
    virtual void render() = 0;

    ///
    /// @brief  Update the Talking Head with current frame
    ///
    virtual void update() = 0;

    ///
    /// @brief  Rewind the Talking Head to the first frame
    ///
    virtual void rewind() = 0;

    ///
    /// @brief  Advance the Talking Head to the next frame
    ///
    virtual void next() = 0;
    ///
    /// @brief  Judge whether it is the end of the Talking Head frames
    ///
    virtual bool isEnd() const = 0;

    ///
    /// @brief  Get the frame rate (FPS: Frames Per Second) of the Talking Head animation
    ///
    virtual int getFPS() const = 0;


public:
    //////////////////////////////////////////////////////////////////////////
    //
    //  Talking Head animation related operations
    //
    //////////////////////////////////////////////////////////////////////////

    ///
    /// @brief  Set the face expression of the Talking Head
    ///
    /// @param  [in] emotionType    Type of the emotion for which control value is set
    /// @param  [in] emotionValue   Control value of the emotion (in range [0, 100])
    ///
    virtual bool setEmotion(ETYPE emotionType, float emotionValue) {return false;}

    ///
    /// @brief  Generate the Talking Head face animation.
    ///         Call this function before playing and rendering the animation.
    ///
    /// @param  [in] fileName       Name of the file which stores the parameters for Talking Head animation
    /// @param  [in] languageTag    Language identification tag ("zh-cmn" for Mandarin, "zh-yue" for Cantonese, "en" for English)
    ///
    virtual bool generateAnimation(const wchar_t *fileName, const wchar_t *languageTag) {return false;}

    ///
    /// @brief  Read the bitmap pixels (only B/G/R 3 bytes for one pixel) from the Talking Head rendering content
    ///
    /// The width and height must be the same as the last resize() or cameraWindow() operation.
    /// Furthermore, the width and height must be aligned to 4 (i.e. width%4 == 0, height%4 == 0).
    /// Otherwise, the read bitmap data might be distorted (misaligned).
    ///
    /// @param  [in]  width         The width of the bitmap to be read
    /// @param  [in]  height        The height of the bitmap to be read
    /// @param  [out] pBitmapBits   Return the read bitmap data
    ///
    virtual bool readBitmapPixels(unsigned char *pBitmapBits, int width, int height) const {return false;}


public:
    //////////////////////////////////////////////////////////////////////////
    //
    //  Talking Head camera related operations (for mouse gestures)
    //
    //////////////////////////////////////////////////////////////////////////

    ///
    /// @brief  Get the camera mode (Zoom, Rotate, or Pan)
    ///
    virtual CMODE cameraModeGet() const {return m_CameraMode;}

    ///
    /// @brief  Set the camera mode (Zoom, Rotate, or Pan)
    ///
    virtual bool cameraModeSet(CMODE mode) {return false;}

    ///
    /// @brief  Mark up the start position of the camera.
    ///         Call this function when mouse left button is clicked.
    ///
    virtual bool cameraStart(int x, int y) {return false;}

    ///
    /// @brief  Mark up the end of the camera updating.
    ///         Call this function when mouse left button is released.
    ///
    virtual bool cameraEnd() {return false;}

    ///
    /// @brief  Update the position of the camera.
    ///         Call this function when mouse is moving
    ///
    virtual bool cameraUpdate(int x, int y) {return false;}

    ///
    /// @brief  Apply the position of the camera updating
    ///
    virtual bool cameraApply() {return false;}

    ///
    /// @brief  Set the screen size of the camera and rendering window.
    ///         Call this function when Talking Head rendering window is resized (e.g. OnSize).
    ///
    virtual bool cameraWindow(int w, int h) {return false;}

    ///
    /// @brief  Set the translation of the camera position
    ///
    virtual bool cameraTranslation(float x, float y, float z) {return false;}

    ///
    /// @brief  Set the rotation of the camera position
    ///
    virtual bool cameraRotation(float x, float y, float z, float angle) {return false;}


protected:
    //////////////////////////////////////////////////////////////////////////
    //
    //  Member variables
    //
    //////////////////////////////////////////////////////////////////////////

    bool    m_bLoaded;      ///< Whether Talking Head model is loaded
    CMODE   m_CameraMode;   ///< Camera mode of the Talking Head (for mouse gesture)
};

#endif//_CST_TALKING_HEAD_H_
