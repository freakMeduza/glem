//#pragma once

//#include "bindable.hpp"

//namespace glem::render {

//    /**
//     * @brief The Texture2D class
//     */
//    class Texture2D : public Bindable {
//    public:
//        Texture2D(uint32_t width, uint32_t height, uint32_t channels, uint32_t slot = 0);

//        ~Texture2D() override;

//        Texture2D(Texture2D&&) = delete;
//        Texture2D(const Texture2D&) = delete;

//        Texture2D& operator=(Texture2D&&) = delete;
//        Texture2D& operator=(const Texture2D&) = delete;

//        /**
//         * @brief fromFile
//         * @param path
//         * @param slot
//         * @return
//         */
//        static std::shared_ptr<Texture2D> fromFile(const std::string& path, uint32_t slot = 0) noexcept;

//        // Bindable interface
//        void bind() noexcept override;
//        void onAppend(Visitor &visitor) noexcept override;

//        /**
//         * @brief width
//         * @return
//         */
//        [[nodiscard]] uint32_t width()    const noexcept;

//        /**
//         * @brief height
//         * @return
//         */
//        [[nodiscard]] uint32_t height()   const noexcept;

//        /**
//         * @brief setData
//         * @param value
//         * @param size
//         */
//        void setData(const void* value, uint32_t size) noexcept;

//    private:
//        uint32_t width_    {0};
//        uint32_t height_   {0};
//        uint32_t channels_ {0};

//        uint32_t slot_ {0};

//    };

//}
