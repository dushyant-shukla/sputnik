#include <string>
#include <inttypes.h>

namespace
{
	GlWrapperAPI api_hook;
} // namespace

using PFNGETGLPROC = void* (const char*);

#define E2S( en ) Enum2String( en ).c_str()
extern std::string Enum2String( GLenum e );

void GlWrapper_glCullFace(GLenum mode)
{
	printf("glCullFace(" "%s)\n", E2S(mode));
	api_hook.glCullFace(mode);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glLineWidth(GLfloat width)
{
	printf("glLineWidth(" "%f)\n", width);
	api_hook.glLineWidth(width);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glPointSize(GLfloat size)
{
	printf("glPointSize(" "%f)\n", size);
	api_hook.glPointSize(size);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glPolygonMode(GLenum face, GLenum mode)
{
	printf("glPolygonMode(" "%s, %s)\n", E2S(face), E2S(mode));
	api_hook.glPolygonMode(face, mode);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
	printf("glScissor(" "%i, %i, %i, %i)\n", x, y, width, height);
	api_hook.glScissor(x, y, width, height);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
	printf("glTexParameterf(" "%s, %s, %f)\n", E2S(target), E2S(pname), param);
	api_hook.glTexParameterf(target, pname, param);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTexParameterfv(GLenum target, GLenum pname, const GLfloat* params)
{
	printf("glTexParameterfv(" "%s, %s, %p)\n", E2S(target), E2S(pname), params);
	api_hook.glTexParameterfv(target, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTexParameteri(GLenum target, GLenum pname, GLint param)
{
	printf("glTexParameteri(" "%s, %s, %i)\n", E2S(target), E2S(pname), param);
	api_hook.glTexParameteri(target, pname, param);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTexParameteriv(GLenum target, GLenum pname, const GLint* params)
{
	printf("glTexParameteriv(" "%s, %s, %p)\n", E2S(target), E2S(pname), params);
	api_hook.glTexParameteriv(target, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels)
{
	printf("glTexImage2D(" "%s, %i, %i, %i, %i, %i, %s, %s, %p)\n", E2S(target), level, internalformat, width, height, border, E2S(format), E2S(type), pixels);
	api_hook.glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glClear(GLbitfield mask)
{
	printf("glClear(" "%u)\n", (unsigned int)(mask));
	api_hook.glClear(mask);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	printf("glClearColor(" "%f, %f, %f, %f)\n", red, green, blue, alpha);
	api_hook.glClearColor(red, green, blue, alpha);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glClearStencil(GLint s)
{
	printf("glClearStencil(" "%i)\n", s);
	api_hook.glClearStencil(s);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glClearDepth(GLdouble depth)
{
	printf("glClearDepth(" "%f)\n", depth);
	api_hook.glClearDepth(depth);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	printf("glColorMask(" "%u, %u, %u, %u)\n", (unsigned int)(red), (unsigned int)(green), (unsigned int)(blue), (unsigned int)(alpha));
	api_hook.glColorMask(red, green, blue, alpha);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glDepthMask(GLboolean flag)
{
	printf("glDepthMask(" "%u)\n", (unsigned int)(flag));
	api_hook.glDepthMask(flag);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glDisable(GLenum cap)
{
	printf("glDisable(" "%s)\n", E2S(cap));
	api_hook.glDisable(cap);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glEnable(GLenum cap)
{
	printf("glEnable(" "%s)\n", E2S(cap));
	api_hook.glEnable(cap);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glFinish()
{
	printf("glFinish()\n");
	api_hook.glFinish();
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glFlush()
{
	printf("glFlush()\n");
	api_hook.glFlush();
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glBlendFunc(GLenum sfactor, GLenum dfactor)
{
	printf("glBlendFunc(" "%s, %s)\n", E2S(sfactor), E2S(dfactor));
	api_hook.glBlendFunc(sfactor, dfactor);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glDepthFunc(GLenum func)
{
	printf("glDepthFunc(" "%s)\n", E2S(func));
	api_hook.glDepthFunc(func);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glPixelStorei(GLenum pname, GLint param)
{
	printf("glPixelStorei(" "%s, %i)\n", E2S(pname), param);
	api_hook.glPixelStorei(pname, param);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glReadBuffer(GLenum src)
{
	printf("glReadBuffer(" "%s)\n", E2S(src));
	api_hook.glReadBuffer(src);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels)
{
	printf("glReadPixels(" "%i, %i, %i, %i, %s, %s, %p)\n", x, y, width, height, E2S(format), E2S(type), pixels);
	api_hook.glReadPixels(x, y, width, height, format, type, pixels);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

GLenum GlWrapper_glGetError()
{
	printf("glGetError()\n");
	GLenum const r = api_hook.glGetError();
	assert(api_hook.glGetError() == GL_NO_ERROR);
	return r;
}

void GlWrapper_glGetIntegerv(GLenum pname, GLint* data)
{
	printf("glGetIntegerv(" "%s, %p)\n", E2S(pname), data);
	api_hook.glGetIntegerv(pname, data);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

const GLubyte* GlWrapper_glGetString(GLenum name)
{
	printf("glGetString(" "%s)\n", E2S(name));
	const GLubyte* const r = api_hook.glGetString(name);
	assert(api_hook.glGetError() == GL_NO_ERROR);
	return r;
}

void GlWrapper_glGetTexImage(GLenum target, GLint level, GLenum format, GLenum type, void* pixels)
{
	printf("glGetTexImage(" "%s, %i, %s, %s, %p)\n", E2S(target), level, E2S(format), E2S(type), pixels);
	api_hook.glGetTexImage(target, level, format, type, pixels);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint* params)
{
	printf("glGetTexLevelParameteriv(" "%s, %i, %s, %p)\n", E2S(target), level, E2S(pname), params);
	api_hook.glGetTexLevelParameteriv(target, level, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	printf("glViewport(" "%i, %i, %i, %i)\n", x, y, width, height);
	api_hook.glViewport(x, y, width, height);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
	printf("glDrawArrays(" "%s, %i, %i)\n", E2S(mode), first, count);
	api_hook.glDrawArrays(mode, first, count);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices)
{
	printf("glDrawElements(" "%s, %i, %s, %p)\n", E2S(mode), count, E2S(type), indices);
	api_hook.glDrawElements(mode, count, type, indices);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels)
{
	printf("glTexSubImage2D(" "%s, %i, %i, %i, %i, %i, %s, %s, %p)\n", E2S(target), level, xoffset, yoffset, width, height, E2S(format), E2S(type), pixels);
	api_hook.glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glDeleteTextures(GLsizei n, const GLuint* textures)
{
	printf("glDeleteTextures(" "%i, %p)\n", n, textures);
	api_hook.glDeleteTextures(n, textures);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGenTextures(GLsizei n, GLuint* textures)
{
	printf("glGenTextures(" "%i, %p)\n", n, textures);
	api_hook.glGenTextures(n, textures);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels)
{
	printf("glTexImage3D(" "%s, %i, %i, %i, %i, %i, %i, %s, %s, %p)\n", E2S(target), level, internalformat, width, height, depth, border, E2S(format), E2S(type), pixels);
	api_hook.glTexImage3D(target, level, internalformat, width, height, depth, border, format, type, pixels);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glActiveTexture(GLenum texture)
{
	printf("glActiveTexture(" "%s)\n", E2S(texture));
	api_hook.glActiveTexture(texture);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data)
{
	printf("glCompressedTexImage3D(" "%s, %i, %s, %i, %i, %i, %i, %i, %p)\n", E2S(target), level, E2S(internalformat), width, height, depth, border, imageSize, data);
	api_hook.glCompressedTexImage3D(target, level, internalformat, width, height, depth, border, imageSize, data);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data)
{
	printf("glCompressedTexImage2D(" "%s, %i, %s, %i, %i, %i, %i, %p)\n", E2S(target), level, E2S(internalformat), width, height, border, imageSize, data);
	api_hook.glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetCompressedTexImage(GLenum target, GLint level, void* img)
{
	printf("glGetCompressedTexImage(" "%s, %i, %p)\n", E2S(target), level, img);
	api_hook.glGetCompressedTexImage(target, level, img);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGenQueries(GLsizei n, GLuint* ids)
{
	printf("glGenQueries(" "%i, %p)\n", n, ids);
	api_hook.glGenQueries(n, ids);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glDeleteQueries(GLsizei n, const GLuint* ids)
{
	printf("glDeleteQueries(" "%i, %p)\n", n, ids);
	api_hook.glDeleteQueries(n, ids);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glBeginQuery(GLenum target, GLuint id)
{
	printf("glBeginQuery(" "%s, %u)\n", E2S(target), id);
	api_hook.glBeginQuery(target, id);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glEndQuery(GLenum target)
{
	printf("glEndQuery(" "%s)\n", E2S(target));
	api_hook.glEndQuery(target);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetQueryObjectiv(GLuint id, GLenum pname, GLint* params)
{
	printf("glGetQueryObjectiv(" "%u, %s, %p)\n", id, E2S(pname), params);
	api_hook.glGetQueryObjectiv(id, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glBindBuffer(GLenum target, GLuint buffer)
{
	printf("glBindBuffer(" "%s, %u)\n", E2S(target), buffer);
	api_hook.glBindBuffer(target, buffer);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glDeleteBuffers(GLsizei n, const GLuint* buffers)
{
	printf("glDeleteBuffers(" "%i, %p)\n", n, buffers);
	api_hook.glDeleteBuffers(n, buffers);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGenBuffers(GLsizei n, GLuint* buffers)
{
	printf("glGenBuffers(" "%i, %p)\n", n, buffers);
	api_hook.glGenBuffers(n, buffers);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
{
	printf("glBufferData(" "%s, %" PRId64", %p, %s)\n", E2S(target), size, data, E2S(usage));
	api_hook.glBufferData(target, size, data, usage);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data)
{
	printf("glBufferSubData(" "%s, %" PRId64", %" PRId64", %p)\n", E2S(target), offset, size, data);
	api_hook.glBufferSubData(target, offset, size, data);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glDrawBuffers(GLsizei n, const GLenum* bufs)
{
	printf("glDrawBuffers(" "%i, %p)\n", n, bufs);
	api_hook.glDrawBuffers(n, bufs);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glAttachShader(GLuint program, GLuint shader)
{
	printf("glAttachShader(" "%u, %u)\n", program, shader);
	api_hook.glAttachShader(program, shader);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glBindAttribLocation(GLuint program, GLuint index, const GLchar* name)
{
	printf("glBindAttribLocation(" "%u, %u, %p)\n", program, index, name);
	api_hook.glBindAttribLocation(program, index, name);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCompileShader(GLuint shader)
{
	printf("glCompileShader(" "%u)\n", shader);
	api_hook.glCompileShader(shader);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

GLuint GlWrapper_glCreateProgram()
{
	printf("glCreateProgram()\n");
	GLuint const r = api_hook.glCreateProgram();
	assert(api_hook.glGetError() == GL_NO_ERROR);
	return r;
}

GLuint GlWrapper_glCreateShader(GLenum type)
{
	printf("glCreateShader(" "%s)\n", E2S(type));
	GLuint const r = api_hook.glCreateShader(type);
	assert(api_hook.glGetError() == GL_NO_ERROR);
	return r;
}

void GlWrapper_glDeleteProgram(GLuint program)
{
	printf("glDeleteProgram(" "%u)\n", program);
	api_hook.glDeleteProgram(program);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glDeleteShader(GLuint shader)
{
	printf("glDeleteShader(" "%u)\n", shader);
	api_hook.glDeleteShader(shader);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glDisableVertexAttribArray(GLuint index)
{
	printf("glDisableVertexAttribArray(" "%u)\n", index);
	api_hook.glDisableVertexAttribArray(index);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glEnableVertexAttribArray(GLuint index)
{
	printf("glEnableVertexAttribArray(" "%u)\n", index);
	api_hook.glEnableVertexAttribArray(index);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name)
{
	printf("glGetActiveAttrib(" "%u, %u, %i, %p, %p, %p, %p)\n", program, index, bufSize, length, size, type, name);
	api_hook.glGetActiveAttrib(program, index, bufSize, length, size, type, name);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name)
{
	printf("glGetActiveUniform(" "%u, %u, %i, %p, %p, %p, %p)\n", program, index, bufSize, length, size, type, name);
	api_hook.glGetActiveUniform(program, index, bufSize, length, size, type, name);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

GLint GlWrapper_glGetAttribLocation(GLuint program, const GLchar* name)
{
	printf("glGetAttribLocation(" "%u, %p)\n", program, name);
	GLint const r = api_hook.glGetAttribLocation(program, name);
	assert(api_hook.glGetError() == GL_NO_ERROR);
	return r;
}

void GlWrapper_glGetProgramiv(GLuint program, GLenum pname, GLint* params)
{
	printf("glGetProgramiv(" "%u, %s, %p)\n", program, E2S(pname), params);
	api_hook.glGetProgramiv(program, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog)
{
	printf("glGetProgramInfoLog(" "%u, %i, %p, %p)\n", program, bufSize, length, infoLog);
	api_hook.glGetProgramInfoLog(program, bufSize, length, infoLog);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetShaderiv(GLuint shader, GLenum pname, GLint* params)
{
	printf("glGetShaderiv(" "%u, %s, %p)\n", shader, E2S(pname), params);
	api_hook.glGetShaderiv(shader, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog)
{
	printf("glGetShaderInfoLog(" "%u, %i, %p, %p)\n", shader, bufSize, length, infoLog);
	api_hook.glGetShaderInfoLog(shader, bufSize, length, infoLog);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

GLint GlWrapper_glGetUniformLocation(GLuint program, const GLchar* name)
{
	printf("glGetUniformLocation(" "%u, %p)\n", program, name);
	GLint const r = api_hook.glGetUniformLocation(program, name);
	assert(api_hook.glGetError() == GL_NO_ERROR);
	return r;
}

GLboolean GlWrapper_glIsProgram(GLuint program)
{
	printf("glIsProgram(" "%u)\n", program);
	GLboolean const r = api_hook.glIsProgram(program);
	assert(api_hook.glGetError() == GL_NO_ERROR);
	return r;
}

GLboolean GlWrapper_glIsShader(GLuint shader)
{
	printf("glIsShader(" "%u)\n", shader);
	GLboolean const r = api_hook.glIsShader(shader);
	assert(api_hook.glGetError() == GL_NO_ERROR);
	return r;
}

void GlWrapper_glLinkProgram(GLuint program)
{
	printf("glLinkProgram(" "%u)\n", program);
	api_hook.glLinkProgram(program);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length)
{
	printf("glShaderSource(" "%u, %i, %p, %p)\n", shader, count, string, length);
	api_hook.glShaderSource(shader, count, string, length);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glUseProgram(GLuint program)
{
	printf("glUseProgram(" "%u)\n", program);
	api_hook.glUseProgram(program);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glUniform1f(GLint location, GLfloat v0)
{
	printf("glUniform1f(" "%i, %f)\n", location, v0);
	api_hook.glUniform1f(location, v0);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glUniform1i(GLint location, GLint v0)
{
	printf("glUniform1i(" "%i, %i)\n", location, v0);
	api_hook.glUniform1i(location, v0);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glUniform1fv(GLint location, GLsizei count, const GLfloat* value)
{
	printf("glUniform1fv(" "%i, %i, %p)\n", location, count, value);
	api_hook.glUniform1fv(location, count, value);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glUniform3fv(GLint location, GLsizei count, const GLfloat* value)
{
	printf("glUniform3fv(" "%i, %i, %p)\n", location, count, value);
	api_hook.glUniform3fv(location, count, value);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glUniform4fv(GLint location, GLsizei count, const GLfloat* value)
{
	printf("glUniform4fv(" "%i, %i, %p)\n", location, count, value);
	api_hook.glUniform4fv(location, count, value);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glUniform1iv(GLint location, GLsizei count, const GLint* value)
{
	printf("glUniform1iv(" "%i, %i, %p)\n", location, count, value);
	api_hook.glUniform1iv(location, count, value);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	printf("glUniformMatrix3fv(" "%i, %i, %u, %p)\n", location, count, (unsigned int)(transpose), value);
	api_hook.glUniformMatrix3fv(location, count, transpose, value);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	printf("glUniformMatrix4fv(" "%i, %i, %u, %p)\n", location, count, (unsigned int)(transpose), value);
	api_hook.glUniformMatrix4fv(location, count, transpose, value);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glValidateProgram(GLuint program)
{
	printf("glValidateProgram(" "%u)\n", program);
	api_hook.glValidateProgram(program);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
{
	printf("glVertexAttribPointer(" "%u, %i, %s, %u, %i, %p)\n", index, size, E2S(type), (unsigned int)(normalized), stride, pointer);
	api_hook.glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glEnablei(GLenum target, GLuint index)
{
	printf("glEnablei(" "%s, %u)\n", E2S(target), index);
	api_hook.glEnablei(target, index);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glDisablei(GLenum target, GLuint index)
{
	printf("glDisablei(" "%s, %u)\n", E2S(target), index);
	api_hook.glDisablei(target, index);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
	printf("glBindBufferRange(" "%s, %u, %u, %" PRId64", %" PRId64")\n", E2S(target), index, buffer, offset, size);
	api_hook.glBindBufferRange(target, index, buffer, offset, size);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glBindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
	printf("glBindBufferBase(" "%s, %u, %u)\n", E2S(target), index, buffer);
	api_hook.glBindBufferBase(target, index, buffer);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glBindFragDataLocation(GLuint program, GLuint color, const GLchar* name)
{
	printf("glBindFragDataLocation(" "%u, %u, %p)\n", program, color, name);
	api_hook.glBindFragDataLocation(program, color, name);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

const GLubyte* GlWrapper_glGetStringi(GLenum name, GLuint index)
{
	printf("glGetStringi(" "%s, %u)\n", E2S(name), index);
	const GLubyte* const r = api_hook.glGetStringi(name, index);
	assert(api_hook.glGetError() == GL_NO_ERROR);
	return r;
}

void GlWrapper_glBindFramebuffer(GLenum target, GLuint framebuffer)
{
	printf("glBindFramebuffer(" "%s, %u)\n", E2S(target), framebuffer);
	api_hook.glBindFramebuffer(target, framebuffer);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers)
{
	printf("glDeleteFramebuffers(" "%i, %p)\n", n, framebuffers);
	api_hook.glDeleteFramebuffers(n, framebuffers);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGenFramebuffers(GLsizei n, GLuint* framebuffers)
{
	printf("glGenFramebuffers(" "%i, %p)\n", n, framebuffers);
	api_hook.glGenFramebuffers(n, framebuffers);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

GLenum GlWrapper_glCheckFramebufferStatus(GLenum target)
{
	printf("glCheckFramebufferStatus(" "%s)\n", E2S(target));
	GLenum const r = api_hook.glCheckFramebufferStatus(target);
	assert(api_hook.glGetError() == GL_NO_ERROR);
	return r;
}

void GlWrapper_glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
	printf("glFramebufferTexture2D(" "%s, %s, %s, %u, %i)\n", E2S(target), E2S(attachment), E2S(textarget), texture, level);
	api_hook.glFramebufferTexture2D(target, attachment, textarget, texture, level);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glFramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
{
	printf("glFramebufferTexture3D(" "%s, %s, %s, %u, %i, %i)\n", E2S(target), E2S(attachment), E2S(textarget), texture, level, zoffset);
	api_hook.glFramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGenerateMipmap(GLenum target)
{
	printf("glGenerateMipmap(" "%s)\n", E2S(target));
	api_hook.glGenerateMipmap(target);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glBindVertexArray(GLuint array)
{
	printf("glBindVertexArray(" "%u)\n", array);
	api_hook.glBindVertexArray(array);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glDeleteVertexArrays(GLsizei n, const GLuint* arrays)
{
	printf("glDeleteVertexArrays(" "%i, %p)\n", n, arrays);
	api_hook.glDeleteVertexArrays(n, arrays);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGenVertexArrays(GLsizei n, GLuint* arrays)
{
	printf("glGenVertexArrays(" "%i, %p)\n", n, arrays);
	api_hook.glGenVertexArrays(n, arrays);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instancecount)
{
	printf("glDrawArraysInstanced(" "%s, %i, %i, %i)\n", E2S(mode), first, count, instancecount);
	api_hook.glDrawArraysInstanced(mode, first, count, instancecount);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount)
{
	printf("glDrawElementsInstanced(" "%s, %i, %s, %p, %i)\n", E2S(mode), count, E2S(type), indices, instancecount);
	api_hook.glDrawElementsInstanced(mode, count, type, indices, instancecount);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params)
{
	printf("glGetActiveUniformBlockiv(" "%u, %u, %s, %p)\n", program, uniformBlockIndex, E2S(pname), params);
	api_hook.glGetActiveUniformBlockiv(program, uniformBlockIndex, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
{
	printf("glUniformBlockBinding(" "%u, %u, %u)\n", program, uniformBlockIndex, uniformBlockBinding);
	api_hook.glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetQueryObjectui64v(GLuint id, GLenum pname, GLuint64* params)
{
	printf("glGetQueryObjectui64v(" "%u, %s, %p)\n", id, E2S(pname), params);
	api_hook.glGetQueryObjectui64v(id, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

GLint GlWrapper_glGetSubroutineUniformLocation(GLuint program, GLenum shadertype, const GLchar* name)
{
	printf("glGetSubroutineUniformLocation(" "%u, %s, %p)\n", program, E2S(shadertype), name);
	GLint const r = api_hook.glGetSubroutineUniformLocation(program, shadertype, name);
	assert(api_hook.glGetError() == GL_NO_ERROR);
	return r;
}

GLuint GlWrapper_glGetSubroutineIndex(GLuint program, GLenum shadertype, const GLchar* name)
{
	printf("glGetSubroutineIndex(" "%u, %s, %p)\n", program, E2S(shadertype), name);
	GLuint const r = api_hook.glGetSubroutineIndex(program, shadertype, name);
	assert(api_hook.glGetError() == GL_NO_ERROR);
	return r;
}

void GlWrapper_glGetActiveSubroutineUniformiv(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint* values)
{
	printf("glGetActiveSubroutineUniformiv(" "%u, %s, %u, %s, %p)\n", program, E2S(shadertype), index, E2S(pname), values);
	api_hook.glGetActiveSubroutineUniformiv(program, shadertype, index, pname, values);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetActiveSubroutineName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei* length, GLchar* name)
{
	printf("glGetActiveSubroutineName(" "%u, %s, %u, %i, %p, %p)\n", program, E2S(shadertype), index, bufsize, length, name);
	api_hook.glGetActiveSubroutineName(program, shadertype, index, bufsize, length, name);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glUniformSubroutinesuiv(GLenum shadertype, GLsizei count, const GLuint* indices)
{
	printf("glUniformSubroutinesuiv(" "%s, %i, %p)\n", E2S(shadertype), count, indices);
	api_hook.glUniformSubroutinesuiv(shadertype, count, indices);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetProgramStageiv(GLuint program, GLenum shadertype, GLenum pname, GLint* values)
{
	printf("glGetProgramStageiv(" "%u, %s, %s, %p)\n", program, E2S(shadertype), E2S(pname), values);
	api_hook.glGetProgramStageiv(program, shadertype, pname, values);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glPatchParameteri(GLenum pname, GLint value)
{
	printf("glPatchParameteri(" "%s, %i)\n", E2S(pname), value);
	api_hook.glPatchParameteri(pname, value);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, void* binary)
{
	printf("glGetProgramBinary(" "%u, %i, %p, %p, %p)\n", program, bufSize, length, binaryFormat, binary);
	api_hook.glGetProgramBinary(program, bufSize, length, binaryFormat, binary);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glProgramBinary(GLuint program, GLenum binaryFormat, const void* binary, GLsizei length)
{
	printf("glProgramBinary(" "%u, %s, %p, %i)\n", program, E2S(binaryFormat), binary, length);
	api_hook.glProgramBinary(program, binaryFormat, binary, length);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glProgramParameteri(GLuint program, GLenum pname, GLint value)
{
	printf("glProgramParameteri(" "%u, %s, %i)\n", program, E2S(pname), value);
	api_hook.glProgramParameteri(program, pname, value);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glProgramUniform1i(GLuint program, GLint location, GLint v0)
{
	printf("glProgramUniform1i(" "%u, %i, %i)\n", program, location, v0);
	api_hook.glProgramUniform1i(program, location, v0);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glProgramUniform1f(GLuint program, GLint location, GLfloat v0)
{
	printf("glProgramUniform1f(" "%u, %i, %f)\n", program, location, v0);
	api_hook.glProgramUniform1f(program, location, v0);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glProgramUniform2iv(GLuint program, GLint location, GLsizei count, const GLint* value)
{
	printf("glProgramUniform2iv(" "%u, %i, %i, %p)\n", program, location, count, value);
	api_hook.glProgramUniform2iv(program, location, count, value);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glProgramUniform2fv(GLuint program, GLint location, GLsizei count, const GLfloat* value)
{
	printf("glProgramUniform2fv(" "%u, %i, %i, %p)\n", program, location, count, value);
	api_hook.glProgramUniform2fv(program, location, count, value);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glProgramUniform3iv(GLuint program, GLint location, GLsizei count, const GLint* value)
{
	printf("glProgramUniform3iv(" "%u, %i, %i, %p)\n", program, location, count, value);
	api_hook.glProgramUniform3iv(program, location, count, value);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glProgramUniform3fv(GLuint program, GLint location, GLsizei count, const GLfloat* value)
{
	printf("glProgramUniform3fv(" "%u, %i, %i, %p)\n", program, location, count, value);
	api_hook.glProgramUniform3fv(program, location, count, value);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glProgramUniform4iv(GLuint program, GLint location, GLsizei count, const GLint* value)
{
	printf("glProgramUniform4iv(" "%u, %i, %i, %p)\n", program, location, count, value);
	api_hook.glProgramUniform4iv(program, location, count, value);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glProgramUniform4fv(GLuint program, GLint location, GLsizei count, const GLfloat* value)
{
	printf("glProgramUniform4fv(" "%u, %i, %i, %p)\n", program, location, count, value);
	api_hook.glProgramUniform4fv(program, location, count, value);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glBindTextures(GLuint first, GLsizei count, const GLuint* textures)
{
	printf("glBindTextures(" "%u, %i, %p)\n", first, count, textures);
	api_hook.glBindTextures(first, count, textures);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCreateTransformFeedbacks(GLsizei n, GLuint* ids)
{
	printf("glCreateTransformFeedbacks(" "%i, %p)\n", n, ids);
	api_hook.glCreateTransformFeedbacks(n, ids);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTransformFeedbackBufferBase(GLuint xfb, GLuint index, GLuint buffer)
{
	printf("glTransformFeedbackBufferBase(" "%u, %u, %u)\n", xfb, index, buffer);
	api_hook.glTransformFeedbackBufferBase(xfb, index, buffer);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTransformFeedbackBufferRange(GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizei size)
{
	printf("glTransformFeedbackBufferRange(" "%u, %u, %u, %" PRId64", %i)\n", xfb, index, buffer, offset, size);
	api_hook.glTransformFeedbackBufferRange(xfb, index, buffer, offset, size);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetTransformFeedbackiv(GLuint xfb, GLenum pname, GLint* param)
{
	printf("glGetTransformFeedbackiv(" "%u, %s, %p)\n", xfb, E2S(pname), param);
	api_hook.glGetTransformFeedbackiv(xfb, pname, param);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetTransformFeedbacki_v(GLuint xfb, GLenum pname, GLuint index, GLint* param)
{
	printf("glGetTransformFeedbacki_v(" "%u, %s, %u, %p)\n", xfb, E2S(pname), index, param);
	api_hook.glGetTransformFeedbacki_v(xfb, pname, index, param);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetTransformFeedbacki64_v(GLuint xfb, GLenum pname, GLuint index, GLint64* param)
{
	printf("glGetTransformFeedbacki64_v(" "%u, %s, %u, %p)\n", xfb, E2S(pname), index, param);
	api_hook.glGetTransformFeedbacki64_v(xfb, pname, index, param);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCreateBuffers(GLsizei n, GLuint* buffers)
{
	printf("glCreateBuffers(" "%i, %p)\n", n, buffers);
	api_hook.glCreateBuffers(n, buffers);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glNamedBufferStorage(GLuint buffer, GLsizei size, const void* data, GLbitfield flags)
{
	printf("glNamedBufferStorage(" "%u, %i, %p, %u)\n", buffer, size, data, (unsigned int)(flags));
	api_hook.glNamedBufferStorage(buffer, size, data, flags);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glNamedBufferData(GLuint buffer, GLsizei size, const void* data, GLenum usage)
{
	printf("glNamedBufferData(" "%u, %i, %p, %s)\n", buffer, size, data, E2S(usage));
	api_hook.glNamedBufferData(buffer, size, data, usage);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizei size, const void* data)
{
	printf("glNamedBufferSubData(" "%u, %" PRId64", %i, %p)\n", buffer, offset, size, data);
	api_hook.glNamedBufferSubData(buffer, offset, size, data);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCopyNamedBufferSubData(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizei size)
{
	printf("glCopyNamedBufferSubData(" "%u, %u, %" PRId64", %" PRId64", %i)\n", readBuffer, writeBuffer, readOffset, writeOffset, size);
	api_hook.glCopyNamedBufferSubData(readBuffer, writeBuffer, readOffset, writeOffset, size);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glClearNamedBufferData(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void* data)
{
	printf("glClearNamedBufferData(" "%u, %s, %s, %s, %p)\n", buffer, E2S(internalformat), E2S(format), E2S(type), data);
	api_hook.glClearNamedBufferData(buffer, internalformat, format, type, data);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glClearNamedBufferSubData(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizei size, GLenum format, GLenum type, const void* data)
{
	printf("glClearNamedBufferSubData(" "%u, %s, %" PRId64", %i, %s, %s, %p)\n", buffer, E2S(internalformat), offset, size, E2S(format), E2S(type), data);
	api_hook.glClearNamedBufferSubData(buffer, internalformat, offset, size, format, type, data);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void* GlWrapper_glMapNamedBuffer(GLuint buffer, GLenum access)
{
	printf("glMapNamedBuffer(" "%u, %s)\n", buffer, E2S(access));
	void* const r = api_hook.glMapNamedBuffer(buffer, access);
	assert(api_hook.glGetError() == GL_NO_ERROR);
	return r;
}

void* GlWrapper_glMapNamedBufferRange(GLuint buffer, GLintptr offset, GLsizei length, GLbitfield access)
{
	printf("glMapNamedBufferRange(" "%u, %" PRId64", %i, %u)\n", buffer, offset, length, (unsigned int)(access));
	void* const r = api_hook.glMapNamedBufferRange(buffer, offset, length, access);
	assert(api_hook.glGetError() == GL_NO_ERROR);
	return r;
}

GLboolean GlWrapper_glUnmapNamedBuffer(GLuint buffer)
{
	printf("glUnmapNamedBuffer(" "%u)\n", buffer);
	GLboolean const r = api_hook.glUnmapNamedBuffer(buffer);
	assert(api_hook.glGetError() == GL_NO_ERROR);
	return r;
}

void GlWrapper_glFlushMappedNamedBufferRange(GLuint buffer, GLintptr offset, GLsizei length)
{
	printf("glFlushMappedNamedBufferRange(" "%u, %" PRId64", %i)\n", buffer, offset, length);
	api_hook.glFlushMappedNamedBufferRange(buffer, offset, length);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetNamedBufferParameteriv(GLuint buffer, GLenum pname, GLint* params)
{
	printf("glGetNamedBufferParameteriv(" "%u, %s, %p)\n", buffer, E2S(pname), params);
	api_hook.glGetNamedBufferParameteriv(buffer, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetNamedBufferParameteri64v(GLuint buffer, GLenum pname, GLint64* params)
{
	printf("glGetNamedBufferParameteri64v(" "%u, %s, %p)\n", buffer, E2S(pname), params);
	api_hook.glGetNamedBufferParameteri64v(buffer, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetNamedBufferPointerv(GLuint buffer, GLenum pname, void** params)
{
	printf("glGetNamedBufferPointerv(" "%u, %s, %p)\n", buffer, E2S(pname), params);
	api_hook.glGetNamedBufferPointerv(buffer, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizei size, void* data)
{
	printf("glGetNamedBufferSubData(" "%u, %" PRId64", %i, %p)\n", buffer, offset, size, data);
	api_hook.glGetNamedBufferSubData(buffer, offset, size, data);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCreateFramebuffers(GLsizei n, GLuint* framebuffers)
{
	printf("glCreateFramebuffers(" "%i, %p)\n", n, framebuffers);
	api_hook.glCreateFramebuffers(n, framebuffers);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glNamedFramebufferRenderbuffer(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
	printf("glNamedFramebufferRenderbuffer(" "%u, %s, %s, %u)\n", framebuffer, E2S(attachment), E2S(renderbuffertarget), renderbuffer);
	api_hook.glNamedFramebufferRenderbuffer(framebuffer, attachment, renderbuffertarget, renderbuffer);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glNamedFramebufferParameteri(GLuint framebuffer, GLenum pname, GLint param)
{
	printf("glNamedFramebufferParameteri(" "%u, %s, %i)\n", framebuffer, E2S(pname), param);
	api_hook.glNamedFramebufferParameteri(framebuffer, pname, param);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glNamedFramebufferTexture(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level)
{
	printf("glNamedFramebufferTexture(" "%u, %s, %u, %i)\n", framebuffer, E2S(attachment), texture, level);
	api_hook.glNamedFramebufferTexture(framebuffer, attachment, texture, level);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glNamedFramebufferTextureLayer(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer)
{
	printf("glNamedFramebufferTextureLayer(" "%u, %s, %u, %i, %i)\n", framebuffer, E2S(attachment), texture, level, layer);
	api_hook.glNamedFramebufferTextureLayer(framebuffer, attachment, texture, level, layer);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glNamedFramebufferDrawBuffer(GLuint framebuffer, GLenum buf)
{
	printf("glNamedFramebufferDrawBuffer(" "%u, %s)\n", framebuffer, E2S(buf));
	api_hook.glNamedFramebufferDrawBuffer(framebuffer, buf);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glNamedFramebufferDrawBuffers(GLuint framebuffer, GLsizei n, const GLenum* bufs)
{
	printf("glNamedFramebufferDrawBuffers(" "%u, %i, %p)\n", framebuffer, n, bufs);
	api_hook.glNamedFramebufferDrawBuffers(framebuffer, n, bufs);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glNamedFramebufferReadBuffer(GLuint framebuffer, GLenum src)
{
	printf("glNamedFramebufferReadBuffer(" "%u, %s)\n", framebuffer, E2S(src));
	api_hook.glNamedFramebufferReadBuffer(framebuffer, src);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glInvalidateNamedFramebufferData(GLuint framebuffer, GLsizei numAttachments, const GLenum* attachments)
{
	printf("glInvalidateNamedFramebufferData(" "%u, %i, %p)\n", framebuffer, numAttachments, attachments);
	api_hook.glInvalidateNamedFramebufferData(framebuffer, numAttachments, attachments);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glInvalidateNamedFramebufferSubData(GLuint framebuffer, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height)
{
	printf("glInvalidateNamedFramebufferSubData(" "%u, %i, %p, %i, %i, %i, %i)\n", framebuffer, numAttachments, attachments, x, y, width, height);
	api_hook.glInvalidateNamedFramebufferSubData(framebuffer, numAttachments, attachments, x, y, width, height);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glClearNamedFramebufferiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint* value)
{
	printf("glClearNamedFramebufferiv(" "%u, %s, %i, %p)\n", framebuffer, E2S(buffer), drawbuffer, value);
	api_hook.glClearNamedFramebufferiv(framebuffer, buffer, drawbuffer, value);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glClearNamedFramebufferuiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint* value)
{
	printf("glClearNamedFramebufferuiv(" "%u, %s, %i, %p)\n", framebuffer, E2S(buffer), drawbuffer, value);
	api_hook.glClearNamedFramebufferuiv(framebuffer, buffer, drawbuffer, value);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glClearNamedFramebufferfv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat* value)
{
	printf("glClearNamedFramebufferfv(" "%u, %s, %i, %p)\n", framebuffer, E2S(buffer), drawbuffer, value);
	api_hook.glClearNamedFramebufferfv(framebuffer, buffer, drawbuffer, value);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glClearNamedFramebufferfi(GLuint framebuffer, GLenum buffer, const GLfloat depth, GLint stencil)
{
	printf("glClearNamedFramebufferfi(" "%u, %s, %f, %i)\n", framebuffer, E2S(buffer), depth, stencil);
	api_hook.glClearNamedFramebufferfi(framebuffer, buffer, depth, stencil);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glBlitNamedFramebuffer(GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
	printf("glBlitNamedFramebuffer(" "%u, %u, %i, %i, %i, %i, %i, %i, %i, %i, %u, %s)\n", readFramebuffer, drawFramebuffer, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, (unsigned int)(mask), E2S(filter));
	api_hook.glBlitNamedFramebuffer(readFramebuffer, drawFramebuffer, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

GLenum GlWrapper_glCheckNamedFramebufferStatus(GLuint framebuffer, GLenum target)
{
	printf("glCheckNamedFramebufferStatus(" "%u, %s)\n", framebuffer, E2S(target));
	GLenum const r = api_hook.glCheckNamedFramebufferStatus(framebuffer, target);
	assert(api_hook.glGetError() == GL_NO_ERROR);
	return r;
}

void GlWrapper_glGetNamedFramebufferParameteriv(GLuint framebuffer, GLenum pname, GLint* param)
{
	printf("glGetNamedFramebufferParameteriv(" "%u, %s, %p)\n", framebuffer, E2S(pname), param);
	api_hook.glGetNamedFramebufferParameteriv(framebuffer, pname, param);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetNamedFramebufferAttachmentParameteriv(GLuint framebuffer, GLenum attachment, GLenum pname, GLint* params)
{
	printf("glGetNamedFramebufferAttachmentParameteriv(" "%u, %s, %s, %p)\n", framebuffer, E2S(attachment), E2S(pname), params);
	api_hook.glGetNamedFramebufferAttachmentParameteriv(framebuffer, attachment, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCreateRenderbuffers(GLsizei n, GLuint* renderbuffers)
{
	printf("glCreateRenderbuffers(" "%i, %p)\n", n, renderbuffers);
	api_hook.glCreateRenderbuffers(n, renderbuffers);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glNamedRenderbufferStorage(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height)
{
	printf("glNamedRenderbufferStorage(" "%u, %s, %i, %i)\n", renderbuffer, E2S(internalformat), width, height);
	api_hook.glNamedRenderbufferStorage(renderbuffer, internalformat, width, height);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glNamedRenderbufferStorageMultisample(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
	printf("glNamedRenderbufferStorageMultisample(" "%u, %i, %s, %i, %i)\n", renderbuffer, samples, E2S(internalformat), width, height);
	api_hook.glNamedRenderbufferStorageMultisample(renderbuffer, samples, internalformat, width, height);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetNamedRenderbufferParameteriv(GLuint renderbuffer, GLenum pname, GLint* params)
{
	printf("glGetNamedRenderbufferParameteriv(" "%u, %s, %p)\n", renderbuffer, E2S(pname), params);
	api_hook.glGetNamedRenderbufferParameteriv(renderbuffer, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCreateTextures(GLenum target, GLsizei n, GLuint* textures)
{
	printf("glCreateTextures(" "%s, %i, %p)\n", E2S(target), n, textures);
	api_hook.glCreateTextures(target, n, textures);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTextureBuffer(GLuint texture, GLenum internalformat, GLuint buffer)
{
	printf("glTextureBuffer(" "%u, %s, %u)\n", texture, E2S(internalformat), buffer);
	api_hook.glTextureBuffer(texture, internalformat, buffer);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTextureBufferRange(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizei size)
{
	printf("glTextureBufferRange(" "%u, %s, %u, %" PRId64", %i)\n", texture, E2S(internalformat), buffer, offset, size);
	api_hook.glTextureBufferRange(texture, internalformat, buffer, offset, size);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTextureStorage1D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width)
{
	printf("glTextureStorage1D(" "%u, %i, %s, %i)\n", texture, levels, E2S(internalformat), width);
	api_hook.glTextureStorage1D(texture, levels, internalformat, width);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTextureStorage2D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
	printf("glTextureStorage2D(" "%u, %i, %s, %i, %i)\n", texture, levels, E2S(internalformat), width, height);
	api_hook.glTextureStorage2D(texture, levels, internalformat, width, height);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTextureStorage3D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
	printf("glTextureStorage3D(" "%u, %i, %s, %i, %i, %i)\n", texture, levels, E2S(internalformat), width, height, depth);
	api_hook.glTextureStorage3D(texture, levels, internalformat, width, height, depth);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTextureStorage2DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
{
	printf("glTextureStorage2DMultisample(" "%u, %i, %s, %i, %i, %u)\n", texture, samples, E2S(internalformat), width, height, (unsigned int)(fixedsamplelocations));
	api_hook.glTextureStorage2DMultisample(texture, samples, internalformat, width, height, fixedsamplelocations);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTextureStorage3DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)
{
	printf("glTextureStorage3DMultisample(" "%u, %i, %s, %i, %i, %i, %u)\n", texture, samples, E2S(internalformat), width, height, depth, (unsigned int)(fixedsamplelocations));
	api_hook.glTextureStorage3DMultisample(texture, samples, internalformat, width, height, depth, fixedsamplelocations);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels)
{
	printf("glTextureSubImage1D(" "%u, %i, %i, %i, %s, %s, %p)\n", texture, level, xoffset, width, E2S(format), E2S(type), pixels);
	api_hook.glTextureSubImage1D(texture, level, xoffset, width, format, type, pixels);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels)
{
	printf("glTextureSubImage2D(" "%u, %i, %i, %i, %i, %i, %s, %s, %p)\n", texture, level, xoffset, yoffset, width, height, E2S(format), E2S(type), pixels);
	api_hook.glTextureSubImage2D(texture, level, xoffset, yoffset, width, height, format, type, pixels);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels)
{
	printf("glTextureSubImage3D(" "%u, %i, %i, %i, %i, %i, %i, %i, %s, %s, %p)\n", texture, level, xoffset, yoffset, zoffset, width, height, depth, E2S(format), E2S(type), pixels);
	api_hook.glTextureSubImage3D(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCompressedTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data)
{
	printf("glCompressedTextureSubImage1D(" "%u, %i, %i, %i, %s, %i, %p)\n", texture, level, xoffset, width, E2S(format), imageSize, data);
	api_hook.glCompressedTextureSubImage1D(texture, level, xoffset, width, format, imageSize, data);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCompressedTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data)
{
	printf("glCompressedTextureSubImage2D(" "%u, %i, %i, %i, %i, %i, %s, %i, %p)\n", texture, level, xoffset, yoffset, width, height, E2S(format), imageSize, data);
	api_hook.glCompressedTextureSubImage2D(texture, level, xoffset, yoffset, width, height, format, imageSize, data);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCompressedTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data)
{
	printf("glCompressedTextureSubImage3D(" "%u, %i, %i, %i, %i, %i, %i, %i, %s, %i, %p)\n", texture, level, xoffset, yoffset, zoffset, width, height, depth, E2S(format), imageSize, data);
	api_hook.glCompressedTextureSubImage3D(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCopyTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
{
	printf("glCopyTextureSubImage1D(" "%u, %i, %i, %i, %i, %i)\n", texture, level, xoffset, x, y, width);
	api_hook.glCopyTextureSubImage1D(texture, level, xoffset, x, y, width);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCopyTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	printf("glCopyTextureSubImage2D(" "%u, %i, %i, %i, %i, %i, %i, %i)\n", texture, level, xoffset, yoffset, x, y, width, height);
	api_hook.glCopyTextureSubImage2D(texture, level, xoffset, yoffset, x, y, width, height);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCopyTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	printf("glCopyTextureSubImage3D(" "%u, %i, %i, %i, %i, %i, %i, %i, %i)\n", texture, level, xoffset, yoffset, zoffset, x, y, width, height);
	api_hook.glCopyTextureSubImage3D(texture, level, xoffset, yoffset, zoffset, x, y, width, height);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTextureParameterf(GLuint texture, GLenum pname, GLfloat param)
{
	printf("glTextureParameterf(" "%u, %s, %f)\n", texture, E2S(pname), param);
	api_hook.glTextureParameterf(texture, pname, param);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTextureParameterfv(GLuint texture, GLenum pname, const GLfloat* param)
{
	printf("glTextureParameterfv(" "%u, %s, %p)\n", texture, E2S(pname), param);
	api_hook.glTextureParameterfv(texture, pname, param);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTextureParameteri(GLuint texture, GLenum pname, GLint param)
{
	printf("glTextureParameteri(" "%u, %s, %i)\n", texture, E2S(pname), param);
	api_hook.glTextureParameteri(texture, pname, param);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTextureParameterIiv(GLuint texture, GLenum pname, const GLint* params)
{
	printf("glTextureParameterIiv(" "%u, %s, %p)\n", texture, E2S(pname), params);
	api_hook.glTextureParameterIiv(texture, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTextureParameterIuiv(GLuint texture, GLenum pname, const GLuint* params)
{
	printf("glTextureParameterIuiv(" "%u, %s, %p)\n", texture, E2S(pname), params);
	api_hook.glTextureParameterIuiv(texture, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glTextureParameteriv(GLuint texture, GLenum pname, const GLint* param)
{
	printf("glTextureParameteriv(" "%u, %s, %p)\n", texture, E2S(pname), param);
	api_hook.glTextureParameteriv(texture, pname, param);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGenerateTextureMipmap(GLuint texture)
{
	printf("glGenerateTextureMipmap(" "%u)\n", texture);
	api_hook.glGenerateTextureMipmap(texture);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glBindTextureUnit(GLuint unit, GLuint texture)
{
	printf("glBindTextureUnit(" "%u, %u)\n", unit, texture);
	api_hook.glBindTextureUnit(unit, texture);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetTextureImage(GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void* pixels)
{
	printf("glGetTextureImage(" "%u, %i, %s, %s, %i, %p)\n", texture, level, E2S(format), E2S(type), bufSize, pixels);
	api_hook.glGetTextureImage(texture, level, format, type, bufSize, pixels);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetCompressedTextureImage(GLuint texture, GLint level, GLsizei bufSize, void* pixels)
{
	printf("glGetCompressedTextureImage(" "%u, %i, %i, %p)\n", texture, level, bufSize, pixels);
	api_hook.glGetCompressedTextureImage(texture, level, bufSize, pixels);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetTextureLevelParameterfv(GLuint texture, GLint level, GLenum pname, GLfloat* params)
{
	printf("glGetTextureLevelParameterfv(" "%u, %i, %s, %p)\n", texture, level, E2S(pname), params);
	api_hook.glGetTextureLevelParameterfv(texture, level, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetTextureLevelParameteriv(GLuint texture, GLint level, GLenum pname, GLint* params)
{
	printf("glGetTextureLevelParameteriv(" "%u, %i, %s, %p)\n", texture, level, E2S(pname), params);
	api_hook.glGetTextureLevelParameteriv(texture, level, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetTextureParameterfv(GLuint texture, GLenum pname, GLfloat* params)
{
	printf("glGetTextureParameterfv(" "%u, %s, %p)\n", texture, E2S(pname), params);
	api_hook.glGetTextureParameterfv(texture, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetTextureParameterIiv(GLuint texture, GLenum pname, GLint* params)
{
	printf("glGetTextureParameterIiv(" "%u, %s, %p)\n", texture, E2S(pname), params);
	api_hook.glGetTextureParameterIiv(texture, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetTextureParameterIuiv(GLuint texture, GLenum pname, GLuint* params)
{
	printf("glGetTextureParameterIuiv(" "%u, %s, %p)\n", texture, E2S(pname), params);
	api_hook.glGetTextureParameterIuiv(texture, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetTextureParameteriv(GLuint texture, GLenum pname, GLint* params)
{
	printf("glGetTextureParameteriv(" "%u, %s, %p)\n", texture, E2S(pname), params);
	api_hook.glGetTextureParameteriv(texture, pname, params);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCreateVertexArrays(GLsizei n, GLuint* arrays)
{
	printf("glCreateVertexArrays(" "%i, %p)\n", n, arrays);
	api_hook.glCreateVertexArrays(n, arrays);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glDisableVertexArrayAttrib(GLuint vaobj, GLuint index)
{
	printf("glDisableVertexArrayAttrib(" "%u, %u)\n", vaobj, index);
	api_hook.glDisableVertexArrayAttrib(vaobj, index);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glEnableVertexArrayAttrib(GLuint vaobj, GLuint index)
{
	printf("glEnableVertexArrayAttrib(" "%u, %u)\n", vaobj, index);
	api_hook.glEnableVertexArrayAttrib(vaobj, index);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glVertexArrayElementBuffer(GLuint vaobj, GLuint buffer)
{
	printf("glVertexArrayElementBuffer(" "%u, %u)\n", vaobj, buffer);
	api_hook.glVertexArrayElementBuffer(vaobj, buffer);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glVertexArrayVertexBuffer(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)
{
	printf("glVertexArrayVertexBuffer(" "%u, %u, %u, %" PRId64", %i)\n", vaobj, bindingindex, buffer, offset, stride);
	api_hook.glVertexArrayVertexBuffer(vaobj, bindingindex, buffer, offset, stride);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glVertexArrayVertexBuffers(GLuint vaobj, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offsets, const GLsizei* strides)
{
	printf("glVertexArrayVertexBuffers(" "%u, %u, %i, %p, %p, %p)\n", vaobj, first, count, buffers, offsets, strides);
	api_hook.glVertexArrayVertexBuffers(vaobj, first, count, buffers, offsets, strides);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glVertexArrayAttribBinding(GLuint vaobj, GLuint attribindex, GLuint bindingindex)
{
	printf("glVertexArrayAttribBinding(" "%u, %u, %u)\n", vaobj, attribindex, bindingindex);
	api_hook.glVertexArrayAttribBinding(vaobj, attribindex, bindingindex);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glVertexArrayAttribFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
{
	printf("glVertexArrayAttribFormat(" "%u, %u, %i, %s, %u, %u)\n", vaobj, attribindex, size, E2S(type), (unsigned int)(normalized), relativeoffset);
	api_hook.glVertexArrayAttribFormat(vaobj, attribindex, size, type, normalized, relativeoffset);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glVertexArrayAttribIFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
	printf("glVertexArrayAttribIFormat(" "%u, %u, %i, %s, %u)\n", vaobj, attribindex, size, E2S(type), relativeoffset);
	api_hook.glVertexArrayAttribIFormat(vaobj, attribindex, size, type, relativeoffset);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glVertexArrayAttribLFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
{
	printf("glVertexArrayAttribLFormat(" "%u, %u, %i, %s, %u)\n", vaobj, attribindex, size, E2S(type), relativeoffset);
	api_hook.glVertexArrayAttribLFormat(vaobj, attribindex, size, type, relativeoffset);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glVertexArrayBindingDivisor(GLuint vaobj, GLuint bindingindex, GLuint divisor)
{
	printf("glVertexArrayBindingDivisor(" "%u, %u, %u)\n", vaobj, bindingindex, divisor);
	api_hook.glVertexArrayBindingDivisor(vaobj, bindingindex, divisor);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetVertexArrayiv(GLuint vaobj, GLenum pname, GLint* param)
{
	printf("glGetVertexArrayiv(" "%u, %s, %p)\n", vaobj, E2S(pname), param);
	api_hook.glGetVertexArrayiv(vaobj, pname, param);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetVertexArrayIndexediv(GLuint vaobj, GLuint index, GLenum pname, GLint* param)
{
	printf("glGetVertexArrayIndexediv(" "%u, %u, %s, %p)\n", vaobj, index, E2S(pname), param);
	api_hook.glGetVertexArrayIndexediv(vaobj, index, pname, param);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glGetVertexArrayIndexed64iv(GLuint vaobj, GLuint index, GLenum pname, GLint64* param)
{
	printf("glGetVertexArrayIndexed64iv(" "%u, %u, %s, %p)\n", vaobj, index, E2S(pname), param);
	api_hook.glGetVertexArrayIndexed64iv(vaobj, index, pname, param);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCreateSamplers(GLsizei n, GLuint* samplers)
{
	printf("glCreateSamplers(" "%i, %p)\n", n, samplers);
	api_hook.glCreateSamplers(n, samplers);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCreateProgramPipelines(GLsizei n, GLuint* pipelines)
{
	printf("glCreateProgramPipelines(" "%i, %p)\n", n, pipelines);
	api_hook.glCreateProgramPipelines(n, pipelines);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

void GlWrapper_glCreateQueries(GLenum target, GLsizei n, GLuint* ids)
{
	printf("glCreateQueries(" "%s, %i, %p)\n", E2S(target), n, ids);
	api_hook.glCreateQueries(target, n, ids);
	assert(api_hook.glGetError() == GL_NO_ERROR);
}

#define INJECT(S) api->S = &GlWrapper_##S;

void InjectWrapperAPI(GlWrapperAPI* api)
{
	api_hook = *api;
	INJECT(glActiveTexture);
	INJECT(glAttachShader);
	INJECT(glBeginQuery);
	INJECT(glBindAttribLocation);
	INJECT(glBindBuffer);
	INJECT(glBindBufferBase);
	INJECT(glBindBufferRange);
	INJECT(glBindFragDataLocation);
	INJECT(glBindFramebuffer);
	INJECT(glBindTextureUnit);
	INJECT(glBindTextures);
	INJECT(glBindVertexArray);
	INJECT(glBlendFunc);
	INJECT(glBlitNamedFramebuffer);
	INJECT(glBufferData);
	INJECT(glBufferSubData);
	INJECT(glCheckFramebufferStatus);
	INJECT(glCheckNamedFramebufferStatus);
	INJECT(glClear);
	INJECT(glClearColor);
	INJECT(glClearDepth);
	INJECT(glClearNamedBufferData);
	INJECT(glClearNamedBufferSubData);
	INJECT(glClearNamedFramebufferfi);
	INJECT(glClearNamedFramebufferfv);
	INJECT(glClearNamedFramebufferiv);
	INJECT(glClearNamedFramebufferuiv);
	INJECT(glClearStencil);
	INJECT(glColorMask);
	INJECT(glCompileShader);
	INJECT(glCompressedTexImage2D);
	INJECT(glCompressedTexImage3D);
	INJECT(glCompressedTextureSubImage1D);
	INJECT(glCompressedTextureSubImage2D);
	INJECT(glCompressedTextureSubImage3D);
	INJECT(glCopyNamedBufferSubData);
	INJECT(glCopyTextureSubImage1D);
	INJECT(glCopyTextureSubImage2D);
	INJECT(glCopyTextureSubImage3D);
	INJECT(glCreateBuffers);
	INJECT(glCreateFramebuffers);
	INJECT(glCreateProgram);
	INJECT(glCreateProgramPipelines);
	INJECT(glCreateQueries);
	INJECT(glCreateRenderbuffers);
	INJECT(glCreateSamplers);
	INJECT(glCreateShader);
	INJECT(glCreateTextures);
	INJECT(glCreateTransformFeedbacks);
	INJECT(glCreateVertexArrays);
	INJECT(glCullFace);
	INJECT(glDeleteBuffers);
	INJECT(glDeleteFramebuffers);
	INJECT(glDeleteProgram);
	INJECT(glDeleteQueries);
	INJECT(glDeleteShader);
	INJECT(glDeleteTextures);
	INJECT(glDeleteVertexArrays);
	INJECT(glDepthFunc);
	INJECT(glDepthMask);
	INJECT(glDisable);
	INJECT(glDisableVertexArrayAttrib);
	INJECT(glDisableVertexAttribArray);
	INJECT(glDisablei);
	INJECT(glDrawArrays);
	INJECT(glDrawArraysInstanced);
	INJECT(glDrawBuffers);
	INJECT(glDrawElements);
	INJECT(glDrawElementsInstanced);
	INJECT(glEnable);
	INJECT(glEnableVertexArrayAttrib);
	INJECT(glEnableVertexAttribArray);
	INJECT(glEnablei);
	INJECT(glEndQuery);
	INJECT(glFinish);
	INJECT(glFlush);
	INJECT(glFlushMappedNamedBufferRange);
	INJECT(glFramebufferTexture2D);
	INJECT(glFramebufferTexture3D);
	INJECT(glGenBuffers);
	INJECT(glGenFramebuffers);
	INJECT(glGenQueries);
	INJECT(glGenTextures);
	INJECT(glGenVertexArrays);
	INJECT(glGenerateMipmap);
	INJECT(glGenerateTextureMipmap);
	INJECT(glGetActiveAttrib);
	INJECT(glGetActiveSubroutineName);
	INJECT(glGetActiveSubroutineUniformiv);
	INJECT(glGetActiveUniform);
	INJECT(glGetActiveUniformBlockiv);
	INJECT(glGetAttribLocation);
	INJECT(glGetCompressedTexImage);
	INJECT(glGetCompressedTextureImage);
	INJECT(glGetIntegerv);
	INJECT(glGetNamedBufferParameteri64v);
	INJECT(glGetNamedBufferParameteriv);
	INJECT(glGetNamedBufferPointerv);
	INJECT(glGetNamedBufferSubData);
	INJECT(glGetNamedFramebufferAttachmentParameteriv);
	INJECT(glGetNamedFramebufferParameteriv);
	INJECT(glGetNamedRenderbufferParameteriv);
	INJECT(glGetProgramBinary);
	INJECT(glGetProgramInfoLog);
	INJECT(glGetProgramStageiv);
	INJECT(glGetProgramiv);
	INJECT(glGetQueryObjectiv);
	INJECT(glGetQueryObjectui64v);
	INJECT(glGetShaderInfoLog);
	INJECT(glGetShaderiv);
	INJECT(glGetSubroutineIndex);
	INJECT(glGetSubroutineUniformLocation);
	INJECT(glGetTexImage);
	INJECT(glGetTexLevelParameteriv);
	INJECT(glGetTextureImage);
	INJECT(glGetTextureLevelParameterfv);
	INJECT(glGetTextureLevelParameteriv);
	INJECT(glGetTextureParameterIiv);
	INJECT(glGetTextureParameterIuiv);
	INJECT(glGetTextureParameterfv);
	INJECT(glGetTextureParameteriv);
	INJECT(glGetTransformFeedbacki64_v);
	INJECT(glGetTransformFeedbacki_v);
	INJECT(glGetTransformFeedbackiv);
	INJECT(glGetUniformLocation);
	INJECT(glGetVertexArrayIndexed64iv);
	INJECT(glGetVertexArrayIndexediv);
	INJECT(glGetVertexArrayiv);
	INJECT(glInvalidateNamedFramebufferData);
	INJECT(glInvalidateNamedFramebufferSubData);
	INJECT(glIsProgram);
	INJECT(glIsShader);
	INJECT(glLineWidth);
	INJECT(glLinkProgram);
	INJECT(glMapNamedBuffer);
	INJECT(glMapNamedBufferRange);
	INJECT(glNamedBufferData);
	INJECT(glNamedBufferStorage);
	INJECT(glNamedBufferSubData);
	INJECT(glNamedFramebufferDrawBuffer);
	INJECT(glNamedFramebufferDrawBuffers);
	INJECT(glNamedFramebufferParameteri);
	INJECT(glNamedFramebufferReadBuffer);
	INJECT(glNamedFramebufferRenderbuffer);
	INJECT(glNamedFramebufferTexture);
	INJECT(glNamedFramebufferTextureLayer);
	INJECT(glNamedRenderbufferStorage);
	INJECT(glNamedRenderbufferStorageMultisample);
	INJECT(glPatchParameteri);
	INJECT(glPixelStorei);
	INJECT(glPointSize);
	INJECT(glPolygonMode);
	INJECT(glProgramBinary);
	INJECT(glProgramParameteri);
	INJECT(glProgramUniform1f);
	INJECT(glProgramUniform1i);
	INJECT(glProgramUniform2fv);
	INJECT(glProgramUniform2iv);
	INJECT(glProgramUniform3fv);
	INJECT(glProgramUniform3iv);
	INJECT(glProgramUniform4fv);
	INJECT(glProgramUniform4iv);
	INJECT(glReadBuffer);
	INJECT(glReadPixels);
	INJECT(glScissor);
	INJECT(glShaderSource);
	INJECT(glTexImage2D);
	INJECT(glTexImage3D);
	INJECT(glTexParameterf);
	INJECT(glTexParameterfv);
	INJECT(glTexParameteri);
	INJECT(glTexParameteriv);
	INJECT(glTexSubImage2D);
	INJECT(glTextureBuffer);
	INJECT(glTextureBufferRange);
	INJECT(glTextureParameterIiv);
	INJECT(glTextureParameterIuiv);
	INJECT(glTextureParameterf);
	INJECT(glTextureParameterfv);
	INJECT(glTextureParameteri);
	INJECT(glTextureParameteriv);
	INJECT(glTextureStorage1D);
	INJECT(glTextureStorage2D);
	INJECT(glTextureStorage2DMultisample);
	INJECT(glTextureStorage3D);
	INJECT(glTextureStorage3DMultisample);
	INJECT(glTextureSubImage1D);
	INJECT(glTextureSubImage2D);
	INJECT(glTextureSubImage3D);
	INJECT(glTransformFeedbackBufferBase);
	INJECT(glTransformFeedbackBufferRange);
	INJECT(glUniform1f);
	INJECT(glUniform1fv);
	INJECT(glUniform1i);
	INJECT(glUniform1iv);
	INJECT(glUniform3fv);
	INJECT(glUniform4fv);
	INJECT(glUniformBlockBinding);
	INJECT(glUniformMatrix3fv);
	INJECT(glUniformMatrix4fv);
	INJECT(glUniformSubroutinesuiv);
	INJECT(glUnmapNamedBuffer);
	INJECT(glUseProgram);
	INJECT(glValidateProgram);
	INJECT(glVertexArrayAttribBinding);
	INJECT(glVertexArrayAttribFormat);
	INJECT(glVertexArrayAttribIFormat);
	INJECT(glVertexArrayAttribLFormat);
	INJECT(glVertexArrayBindingDivisor);
	INJECT(glVertexArrayElementBuffer);
	INJECT(glVertexArrayVertexBuffer);
	INJECT(glVertexArrayVertexBuffers);
	INJECT(glVertexAttribPointer);
	INJECT(glViewport);
}

#define LOAD_GL_FUNC(func) api->func = ( decltype(api->func) )GetGLProc(#func);

void GetGlAPI(GlWrapperAPI* api, PFNGETGLPROC GetGLProc)
{
	LOAD_GL_FUNC(glActiveTexture);
	LOAD_GL_FUNC(glAttachShader);
	LOAD_GL_FUNC(glBeginQuery);
	LOAD_GL_FUNC(glBindAttribLocation);
	LOAD_GL_FUNC(glBindBuffer);
	LOAD_GL_FUNC(glBindBufferBase);
	LOAD_GL_FUNC(glBindBufferRange);
	LOAD_GL_FUNC(glBindFragDataLocation);
	LOAD_GL_FUNC(glBindFramebuffer);
	LOAD_GL_FUNC(glBindTextureUnit);
	LOAD_GL_FUNC(glBindTextures);
	LOAD_GL_FUNC(glBindVertexArray);
	LOAD_GL_FUNC(glBlendFunc);
	LOAD_GL_FUNC(glBlitNamedFramebuffer);
	LOAD_GL_FUNC(glBufferData);
	LOAD_GL_FUNC(glBufferSubData);
	LOAD_GL_FUNC(glCheckFramebufferStatus);
	LOAD_GL_FUNC(glCheckNamedFramebufferStatus);
	LOAD_GL_FUNC(glClear);
	LOAD_GL_FUNC(glClearColor);
	LOAD_GL_FUNC(glClearDepth);
	LOAD_GL_FUNC(glClearNamedBufferData);
	LOAD_GL_FUNC(glClearNamedBufferSubData);
	LOAD_GL_FUNC(glClearNamedFramebufferfi);
	LOAD_GL_FUNC(glClearNamedFramebufferfv);
	LOAD_GL_FUNC(glClearNamedFramebufferiv);
	LOAD_GL_FUNC(glClearNamedFramebufferuiv);
	LOAD_GL_FUNC(glClearStencil);
	LOAD_GL_FUNC(glColorMask);
	LOAD_GL_FUNC(glCompileShader);
	LOAD_GL_FUNC(glCompressedTexImage2D);
	LOAD_GL_FUNC(glCompressedTexImage3D);
	LOAD_GL_FUNC(glCompressedTextureSubImage1D);
	LOAD_GL_FUNC(glCompressedTextureSubImage2D);
	LOAD_GL_FUNC(glCompressedTextureSubImage3D);
	LOAD_GL_FUNC(glCopyNamedBufferSubData);
	LOAD_GL_FUNC(glCopyTextureSubImage1D);
	LOAD_GL_FUNC(glCopyTextureSubImage2D);
	LOAD_GL_FUNC(glCopyTextureSubImage3D);
	LOAD_GL_FUNC(glCreateBuffers);
	LOAD_GL_FUNC(glCreateFramebuffers);
	LOAD_GL_FUNC(glCreateProgram);
	LOAD_GL_FUNC(glCreateProgramPipelines);
	LOAD_GL_FUNC(glCreateQueries);
	LOAD_GL_FUNC(glCreateRenderbuffers);
	LOAD_GL_FUNC(glCreateSamplers);
	LOAD_GL_FUNC(glCreateShader);
	LOAD_GL_FUNC(glCreateTextures);
	LOAD_GL_FUNC(glCreateTransformFeedbacks);
	LOAD_GL_FUNC(glCreateVertexArrays);
	LOAD_GL_FUNC(glCullFace);
	LOAD_GL_FUNC(glDeleteBuffers);
	LOAD_GL_FUNC(glDeleteFramebuffers);
	LOAD_GL_FUNC(glDeleteProgram);
	LOAD_GL_FUNC(glDeleteQueries);
	LOAD_GL_FUNC(glDeleteShader);
	LOAD_GL_FUNC(glDeleteTextures);
	LOAD_GL_FUNC(glDeleteVertexArrays);
	LOAD_GL_FUNC(glDepthFunc);
	LOAD_GL_FUNC(glDepthMask);
	LOAD_GL_FUNC(glDisable);
	LOAD_GL_FUNC(glDisableVertexArrayAttrib);
	LOAD_GL_FUNC(glDisableVertexAttribArray);
	LOAD_GL_FUNC(glDisablei);
	LOAD_GL_FUNC(glDrawArrays);
	LOAD_GL_FUNC(glDrawArraysInstanced);
	LOAD_GL_FUNC(glDrawBuffers);
	LOAD_GL_FUNC(glDrawElements);
	LOAD_GL_FUNC(glDrawElementsInstanced);
	LOAD_GL_FUNC(glEnable);
	LOAD_GL_FUNC(glEnableVertexArrayAttrib);
	LOAD_GL_FUNC(glEnableVertexAttribArray);
	LOAD_GL_FUNC(glEnablei);
	LOAD_GL_FUNC(glEndQuery);
	LOAD_GL_FUNC(glFinish);
	LOAD_GL_FUNC(glFlush);
	LOAD_GL_FUNC(glFlushMappedNamedBufferRange);
	LOAD_GL_FUNC(glFramebufferTexture2D);
	LOAD_GL_FUNC(glFramebufferTexture3D);
	LOAD_GL_FUNC(glGenBuffers);
	LOAD_GL_FUNC(glGenFramebuffers);
	LOAD_GL_FUNC(glGenQueries);
	LOAD_GL_FUNC(glGenTextures);
	LOAD_GL_FUNC(glGenVertexArrays);
	LOAD_GL_FUNC(glGenerateMipmap);
	LOAD_GL_FUNC(glGenerateTextureMipmap);
	LOAD_GL_FUNC(glGetActiveAttrib);
	LOAD_GL_FUNC(glGetActiveSubroutineName);
	LOAD_GL_FUNC(glGetActiveSubroutineUniformiv);
	LOAD_GL_FUNC(glGetActiveUniform);
	LOAD_GL_FUNC(glGetActiveUniformBlockiv);
	LOAD_GL_FUNC(glGetAttribLocation);
	LOAD_GL_FUNC(glGetCompressedTexImage);
	LOAD_GL_FUNC(glGetCompressedTextureImage);
	LOAD_GL_FUNC(glGetError);
	LOAD_GL_FUNC(glGetIntegerv);
	LOAD_GL_FUNC(glGetNamedBufferParameteri64v);
	LOAD_GL_FUNC(glGetNamedBufferParameteriv);
	LOAD_GL_FUNC(glGetNamedBufferPointerv);
	LOAD_GL_FUNC(glGetNamedBufferSubData);
	LOAD_GL_FUNC(glGetNamedFramebufferAttachmentParameteriv);
	LOAD_GL_FUNC(glGetNamedFramebufferParameteriv);
	LOAD_GL_FUNC(glGetNamedRenderbufferParameteriv);
	LOAD_GL_FUNC(glGetProgramBinary);
	LOAD_GL_FUNC(glGetProgramInfoLog);
	LOAD_GL_FUNC(glGetProgramStageiv);
	LOAD_GL_FUNC(glGetProgramiv);
	LOAD_GL_FUNC(glGetQueryObjectiv);
	LOAD_GL_FUNC(glGetQueryObjectui64v);
	LOAD_GL_FUNC(glGetShaderInfoLog);
	LOAD_GL_FUNC(glGetShaderiv);
	LOAD_GL_FUNC(glGetSubroutineIndex);
	LOAD_GL_FUNC(glGetSubroutineUniformLocation);
	LOAD_GL_FUNC(glGetTexImage);
	LOAD_GL_FUNC(glGetTexLevelParameteriv);
	LOAD_GL_FUNC(glGetTextureImage);
	LOAD_GL_FUNC(glGetTextureLevelParameterfv);
	LOAD_GL_FUNC(glGetTextureLevelParameteriv);
	LOAD_GL_FUNC(glGetTextureParameterIiv);
	LOAD_GL_FUNC(glGetTextureParameterIuiv);
	LOAD_GL_FUNC(glGetTextureParameterfv);
	LOAD_GL_FUNC(glGetTextureParameteriv);
	LOAD_GL_FUNC(glGetTransformFeedbacki64_v);
	LOAD_GL_FUNC(glGetTransformFeedbacki_v);
	LOAD_GL_FUNC(glGetTransformFeedbackiv);
	LOAD_GL_FUNC(glGetUniformLocation);
	LOAD_GL_FUNC(glGetVertexArrayIndexed64iv);
	LOAD_GL_FUNC(glGetVertexArrayIndexediv);
	LOAD_GL_FUNC(glGetVertexArrayiv);
	LOAD_GL_FUNC(glInvalidateNamedFramebufferData);
	LOAD_GL_FUNC(glInvalidateNamedFramebufferSubData);
	LOAD_GL_FUNC(glIsProgram);
	LOAD_GL_FUNC(glIsShader);
	LOAD_GL_FUNC(glLineWidth);
	LOAD_GL_FUNC(glLinkProgram);
	LOAD_GL_FUNC(glMapNamedBuffer);
	LOAD_GL_FUNC(glMapNamedBufferRange);
	LOAD_GL_FUNC(glNamedBufferData);
	LOAD_GL_FUNC(glNamedBufferStorage);
	LOAD_GL_FUNC(glNamedBufferSubData);
	LOAD_GL_FUNC(glNamedFramebufferDrawBuffer);
	LOAD_GL_FUNC(glNamedFramebufferDrawBuffers);
	LOAD_GL_FUNC(glNamedFramebufferParameteri);
	LOAD_GL_FUNC(glNamedFramebufferReadBuffer);
	LOAD_GL_FUNC(glNamedFramebufferRenderbuffer);
	LOAD_GL_FUNC(glNamedFramebufferTexture);
	LOAD_GL_FUNC(glNamedFramebufferTextureLayer);
	LOAD_GL_FUNC(glNamedRenderbufferStorage);
	LOAD_GL_FUNC(glNamedRenderbufferStorageMultisample);
	LOAD_GL_FUNC(glPatchParameteri);
	LOAD_GL_FUNC(glPixelStorei);
	LOAD_GL_FUNC(glPointSize);
	LOAD_GL_FUNC(glPolygonMode);
	LOAD_GL_FUNC(glProgramBinary);
	LOAD_GL_FUNC(glProgramParameteri);
	LOAD_GL_FUNC(glProgramUniform1f);
	LOAD_GL_FUNC(glProgramUniform1i);
	LOAD_GL_FUNC(glProgramUniform2fv);
	LOAD_GL_FUNC(glProgramUniform2iv);
	LOAD_GL_FUNC(glProgramUniform3fv);
	LOAD_GL_FUNC(glProgramUniform3iv);
	LOAD_GL_FUNC(glProgramUniform4fv);
	LOAD_GL_FUNC(glProgramUniform4iv);
	LOAD_GL_FUNC(glReadBuffer);
	LOAD_GL_FUNC(glReadPixels);
	LOAD_GL_FUNC(glScissor);
	LOAD_GL_FUNC(glShaderSource);
	LOAD_GL_FUNC(glTexImage2D);
	LOAD_GL_FUNC(glTexImage3D);
	LOAD_GL_FUNC(glTexParameterf);
	LOAD_GL_FUNC(glTexParameterfv);
	LOAD_GL_FUNC(glTexParameteri);
	LOAD_GL_FUNC(glTexParameteriv);
	LOAD_GL_FUNC(glTexSubImage2D);
	LOAD_GL_FUNC(glTextureBuffer);
	LOAD_GL_FUNC(glTextureBufferRange);
	LOAD_GL_FUNC(glTextureParameterIiv);
	LOAD_GL_FUNC(glTextureParameterIuiv);
	LOAD_GL_FUNC(glTextureParameterf);
	LOAD_GL_FUNC(glTextureParameterfv);
	LOAD_GL_FUNC(glTextureParameteri);
	LOAD_GL_FUNC(glTextureParameteriv);
	LOAD_GL_FUNC(glTextureStorage1D);
	LOAD_GL_FUNC(glTextureStorage2D);
	LOAD_GL_FUNC(glTextureStorage2DMultisample);
	LOAD_GL_FUNC(glTextureStorage3D);
	LOAD_GL_FUNC(glTextureStorage3DMultisample);
	LOAD_GL_FUNC(glTextureSubImage1D);
	LOAD_GL_FUNC(glTextureSubImage2D);
	LOAD_GL_FUNC(glTextureSubImage3D);
	LOAD_GL_FUNC(glTransformFeedbackBufferBase);
	LOAD_GL_FUNC(glTransformFeedbackBufferRange);
	LOAD_GL_FUNC(glUniform1f);
	LOAD_GL_FUNC(glUniform1fv);
	LOAD_GL_FUNC(glUniform1i);
	LOAD_GL_FUNC(glUniform1iv);
	LOAD_GL_FUNC(glUniform3fv);
	LOAD_GL_FUNC(glUniform4fv);
	LOAD_GL_FUNC(glUniformBlockBinding);
	LOAD_GL_FUNC(glUniformMatrix3fv);
	LOAD_GL_FUNC(glUniformMatrix4fv);
	LOAD_GL_FUNC(glUniformSubroutinesuiv);
	LOAD_GL_FUNC(glUnmapNamedBuffer);
	LOAD_GL_FUNC(glUseProgram);
	LOAD_GL_FUNC(glValidateProgram);
	LOAD_GL_FUNC(glVertexArrayAttribBinding);
	LOAD_GL_FUNC(glVertexArrayAttribFormat);
	LOAD_GL_FUNC(glVertexArrayAttribIFormat);
	LOAD_GL_FUNC(glVertexArrayAttribLFormat);
	LOAD_GL_FUNC(glVertexArrayBindingDivisor);
	LOAD_GL_FUNC(glVertexArrayElementBuffer);
	LOAD_GL_FUNC(glVertexArrayVertexBuffer);
	LOAD_GL_FUNC(glVertexArrayVertexBuffers);
	LOAD_GL_FUNC(glVertexAttribPointer);
	LOAD_GL_FUNC(glViewport);
}

