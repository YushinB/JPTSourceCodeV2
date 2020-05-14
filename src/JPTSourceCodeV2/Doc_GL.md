# Document for Graphic-OpenGL

* Shader Class manage Shader files such as *shader vertex, shader Fragment...so on*.

```cpp
class CShader
{
public:
	CShader():vertexId(0), fragmentId(0), shaderId(0){ ; }
	virtual ~CShader();

	GLint compile(const GLchar* vertexSource, const GLchar* fragmentSource,
					 const GLchar* geometrySource = nullptr);

private:
	GLuint vertexId;
	GLuint fragmentId;
	GLuint shaderId;
	CString buildError;
	bool enabled;
}
```
##  AVIStreamWrite function: ([ref](https://docs.microsoft.com/en-us/windows/desktop/api/vfw/nf-vfw-avistreamwrite))
*  The AVIStreamWrite function writes data to a stream.
```cpp
	HRESULT AVIStreamWrite(
	PAVISTREAM pavi,
	LONG       lStart,
	LONG       lSamples,
	LPVOID     lpBuffer,
	LONG       cbBuffer,
	DWORD      dwFlags,
	LONG       *plSampWritten,
	LONG       *plBytesWritten
	);
```

* pavi: Handle to an open stream.
* lSamples: Number of samples to write.
* lpBuffer: Pointer to a buffer containing the data to write.
* cbBuffer: Size of the buffer referenced by lpBuffer.
* dwFlags: Flag associated with this data. The following flag is defined:

## Camera/View space
* look at in OpenGL: ([Link](https://learnopengl.com/Getting-started/Camera))
    ![Look at parameter](res/Doc2.png)
* Parameters (*Variables	Description*)
     *eyeX, eyeY, eyeZ*: Specifies the position of the eye point.
     *centerX, centerY*, centerZ: Specifies the position of the reference point.
     *upX, upY, upZ* : Specifies the direction of the up vector.

```cpp
    glm::mat4 view;
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), 
            glm::vec3(0.0f, 0.0f, 0.0f), 
            glm::vec3(0.0f, 1.0f, 0.0f));
```
## Multi-Threading
