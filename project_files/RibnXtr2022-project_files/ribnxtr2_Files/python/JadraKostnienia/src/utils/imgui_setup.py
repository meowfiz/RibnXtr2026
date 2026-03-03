# -*- coding: utf-8 -*-

from imgui.integrations.glfw import GlfwRenderer
import OpenGL.GL as gl
import glfw
import imgui
import sys

class ImguiWindow:

    def __init__(self, winname="Window", size=(640, 480)):
        self.winname = winname
        self.size = size

        imgui.create_context()
        self.window = self.impl_glfw_init()
        self.impl = GlfwRenderer(self.window)


    def begin_frame(self):
        glfw.poll_events()
        self.impl.process_inputs()
        imgui.new_frame()


    def end_frame(self):
        imgui.render()
        imgui.end_frame()

        gl.glClearColor(1.0, 1.0, 1.0, 1)
        gl.glClear(gl.GL_COLOR_BUFFER_BIT)

        imgui.render()
        self.impl.render(imgui.get_draw_data())
        glfw.swap_buffers(self.window)

    def cleanup(self):
        self.impl.shutdown()
        glfw.terminate()

    def should_close(self):
        return glfw.window_should_close(self.window)

    def impl_glfw_init(self):
        width, height = self.size

        if not glfw.init():
            print("Could not initialize OpenGL context")
            sys.exit(1)

        # OS X supports only forward-compatible core profiles from 3.2
        glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 3)
        glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 3)
        glfw.window_hint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)

        glfw.window_hint(glfw.OPENGL_FORWARD_COMPAT, gl.GL_TRUE)

        # Create a windowed mode window and its OpenGL context
        window = glfw.create_window(int(width), int(height), self.winname, None, None)
        glfw.make_context_current(window)

        if not window:
            glfw.terminate()
            print("Could not initialize Window")
            sys.exit(1)

        return window

if __name__ == "__main__":
    
    #usage example

    window = ImguiWindow()
    while not window.should_close():
        window.begin_frame()
        imgui.text("Hello world!")
        window.end_frame()