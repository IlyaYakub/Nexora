from conan import ConanFile


class NexoraConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("spdlog/1.16.0")
        self.requires("glfw/3.4")
        self.requires("gtest/1.17.0")
        self.requires("vulkan-loader/1.4.357.0")
        self.requires("vk-bootstrap/1.4.350")
        self.requires("vulkan-headers/1.4.357.0", override=True)
