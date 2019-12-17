//#include "texture2D.hpp"

//#include <glad/glad.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

//#include <assert.h>
//#include <iostream>
//#include <optional>
//#include <tuple>

//namespace {
//    [[maybe_unused]] std::optional<std::tuple<int ,int ,int, const void*>> loadImage(const std::string& path) noexcept {
//        int width     {0};
//        int height    {0};
//        int channels  {0};

//        auto pixels = stbi_load(path.data(), &width, &height, &channels, 0);

//        if(!pixels) {
//            std::cerr << "Unable to load image " << path << "." << std::endl;

//            return {};
//        }

//        return std::tuple<int ,int ,int, const void*>{width, height, channels, pixels};
//    }
//}

//namespace glem::render {

//    Texture2D::Texture2D(uint32_t width, uint32_t height, uint32_t channels, uint32_t slot) :
//        width_    {width},
//        height_   {height},
//        channels_ {channels},
//        slot_     {slot}
//    {
//        glCreateTextures(GL_TEXTURE_2D, 1, &id_);

//        GLenum format = (channels_ == 3 ? GL_RGB8 : GL_RGBA8);

//        glTextureStorage2D(id_, 1, format, static_cast<GLsizei>(width_), static_cast<GLsizei>(height_));

//        glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//        glTextureParameteri(id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTextureParameteri(id_, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    }

//    Texture2D::~Texture2D()
//    {
//        glDeleteTextures(1, &id_);
//    }

//    std::shared_ptr<Texture2D> Texture2D::fromFile(const std::string &path, uint32_t slot) noexcept
//    {
//        if(const auto& image = loadImage(path)) {
//            auto [width, height, channels, pixels] = *image;

//            std::cout << "width:    " << width    << std::endl;
//            std::cout << "height:   " << height   << std::endl;
//            std::cout << "channels: " << channels << std::endl;

//            auto texture = std::make_shared<Texture2D>(width, height, channels, slot);

//            texture->setData(pixels, static_cast<uint32_t>(width * height * channels));

//            return texture;
//        }

//        std::cerr << "Unable to create texture." << std::endl;

//        return {};
//    }

//    void Texture2D::bind() noexcept
//    {
//        glBindTextureUnit(slot_, id_);
//    }

//    void Texture2D::onAppend(Visitor &visitor) noexcept
//    {
//        static_cast<void>(visitor);
//    }

//    uint32_t Texture2D::width() const noexcept
//    {
//        return width_;
//    }

//    uint32_t Texture2D::height() const noexcept
//    {
//        return height_;
//    }

//    void Texture2D::setData(const void *value, uint32_t size) noexcept
//    {
//        GLenum format = (channels_ == 3 ? GL_RGB : GL_RGBA);

//        assert((size == width_ * height_ * channels_) && "Value must be entire texture.");

//        glTextureSubImage2D(id_, 0, 0, 0, static_cast<GLsizei>(width_), static_cast<GLsizei>(height_), format, GL_UNSIGNED_BYTE, value);
//    }

//}
