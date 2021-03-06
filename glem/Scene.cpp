#include "Scene.hpp"

#include "Input.hpp"
#include "Camera.hpp"

#include "Window.hpp"
#include "Context.hpp"
#include "Application.hpp"

#include "Buffer.hpp"
#include "Shader.hpp"
#include "Program.hpp"

#include "Image.hpp"
#include "Texture.hpp"

#include "Log.hpp"

#include "Mesh.hpp"
#include "Particle.hpp"
//#include "Primitives.hpp"

namespace {
    static constexpr const char* TAG = "Scene";
}

namespace glem {

    /**** ParticleScene ****/
    ParticleScene::ParticleScene()
    {
        attach();
    }

    ParticleScene::~ParticleScene()
    {
        detach();
    }

    void ParticleScene::attach() noexcept
    {
        const auto& vs = R"glsl(
                         #version 450
                         layout(location = 0) in vec3 vPosition;
                         layout(location = 1) in vec3 vNormal;
                            uniform mat4 uProjectionMatrix;
                            uniform mat4 uViewMatrix;
                            uniform mat4 uModelMatrix = mat4(1.0f);
                         void main() {
                            gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(vPosition, 1.0f);
                         }
                         )glsl";

        const auto& ps = R"glsl(
                         #version 450
                         layout(location = 0) out vec4 FragmentColor;
                            uniform vec4 uColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
                         void main() {
                            FragmentColor = uColor;
                         }
                         )glsl";

        lightProgram_ = std::make_shared<Program>();
        lightProgram_->append(std::make_unique<Shader>(vs, ShaderType::VS));
        lightProgram_->append(std::make_unique<Shader>(ps, ShaderType::PS));

        if(!lightProgram_->link()) {
            Log::e(TAG, "Failed to link light shader program.");
            return;
        }

        const auto& phong_vs = R"glsl(
                         #version 450
                         layout(location = 0) in vec3 vPosition;
                         layout(location = 1) in vec3 vNormal;
                         uniform mat4 uProjectionMatrix;
                         uniform mat4 uViewMatrix;
                         uniform mat4 uModelMatrix = mat4(1.0f);
                         out vec3 fPosition;
                         out vec3 fNormal;
                         void main() {
                            gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(vPosition, 1.0f);
                            fPosition = vec3(uModelMatrix * vec4(vPosition, 1.0f));
                            fNormal   = mat3(transpose(inverse(uModelMatrix))) * vNormal;
                         }
                         )glsl";

        const auto& phong_ps = R"glsl(
                         #version 450
                         layout(location = 0) out vec4 FragmentColor;
                         uniform vec3 uViewPosition;
                         struct Material {
                            vec4 color;
                            float shininess;
                         };
                         struct Light {
                            vec3 position;
                            vec3 ambient;
                            vec3 diffuse;
                            vec3 specular;
                            float constant;
                            float linear;
                            float quadratic;
                         };
                         uniform Light uLight;
                         uniform Material uMaterial;
                         in vec3 fPosition;
                         in vec3 fNormal;
                         void main() {
                            vec3 ambient = uLight.ambient;
                            vec3 N = normalize(fNormal);
                            vec3 L = normalize(uLight.position - fPosition);
                            vec3 diffuse = uLight.diffuse * max(dot(N, L), 0.0f);
                            vec3 V = normalize(uViewPosition - fPosition);
                            vec3 R = reflect(-L, N);
                            vec3 specular = uLight.specular * pow(max(dot(V, R), 0.0f), uMaterial.shininess);
                            float D = length(uLight.position - fPosition);
                            float attenuation = 1.0f / (uLight.constant + uLight.linear * D + uLight.quadratic * D * D);
                            ambient  *= attenuation;
                            diffuse  *= attenuation;
                            specular *= attenuation;
                            FragmentColor = vec4(ambient + diffuse + specular, 1.0f) * uMaterial.color;
                         }
                         )glsl";

        modelProgram_ = std::make_shared<Program>();
        modelProgram_->append(std::make_unique<Shader>(phong_vs, ShaderType::VS));
        modelProgram_->append(std::make_unique<Shader>(phong_ps, ShaderType::PS));

        if(!modelProgram_->link()) {
            Log::e(TAG, "Failed to link model shader program.");
            return;
        }

        modelProgram_->bind();

        /**** material setup ****/
        modelProgram_->setUniform("uMaterial.color",     glm::vec4{1.0f, 0.0f, 1.0f, 1.0f});
        modelProgram_->setUniform("uMaterial.shininess", 32.0f);

        /**** light setup ****/
        modelProgram_->setUniform("uLight.position",  lightPosition_);
        modelProgram_->setUniform("uLight.ambient",   glm::vec3{0.2f, 0.2f, 0.2f});
        modelProgram_->setUniform("uLight.diffuse",   glm::vec3{0.5f, 0.5f, 0.5f});
        modelProgram_->setUniform("uLight.specular",  glm::vec3{1.0f, 1.0f, 1.0f});
        modelProgram_->setUniform("uLight.constant",  1.0f);
        modelProgram_->setUniform("uLight.linear",    0.09f);
        modelProgram_->setUniform("uLight.quadratic", 0.032f);

        const auto projection = glm::perspective(glm::radians(45.0f),
                                                 static_cast<float>(glem::Application::instance().window().width()) / static_cast<float>(glem::Application::instance().window().height()),
                                                 0.1f,
                                                 1000.0f);

        camera_ = std::make_unique<MayaCamera>();
        camera_->setPosition({0.0f, 0.0f, 50.0f});
        camera_->setProjection(projection);

        /**** init vertex arrays ****/
        struct Vertex {
            glm::vec3 position {glm::vec3{0.0f}};
            glm::vec3 normal   {glm::vec3{0.0f}};
        };

        //auto cube = ::primitives::Cube<Vertex>::create();
        auto cube = Shape::cube();

        cube.transform(glm::scale(glm::mat4{1.0f}, glm::vec3{0.5f}));

        cube.setFlat();

        modelVertexArray_ = std::make_shared<VertexArray>();
        modelVertexArray_->append(std::make_unique<VertexBuffer>(cube.buffer));
        modelVertexArray_->append(std::make_unique<IndexBuffer>(cube.indices));

        //auto sphere = ::primitives::Sphere<Vertex>::create({12, 12});
        auto sphere = Shape::sphere({12, 12});

        sphere.transform(glm::scale(glm::mat4{1.0f}, glm::vec3{0.2f}));

        lightVertexArray_ = std::make_shared<VertexArray>();
        lightVertexArray_->append(std::make_unique<VertexBuffer>(sphere.buffer));
        lightVertexArray_->append(std::make_unique<IndexBuffer>(sphere.indices));

        emitter_ = std::make_unique<ParticleEmitter>();

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        setVisible(true);
    }

    void ParticleScene::detach() noexcept
    {
        setVisible(false);
    }

    void ParticleScene::update(float deltaTime) noexcept
    {
        if(visible()) {
            if(Keyboard::pressed(Keyboard::Key::F1)) {
                const auto projection = glm::perspective(glm::radians(45.0f),
                                                         static_cast<float>(glem::Application::instance().window().width()) / static_cast<float>(glem::Application::instance().window().height()),
                                                         0.1f,
                                                         1000.0f);

                camera_.reset(new FreeCamera{});
                camera_->setProjection(projection);
                camera_->setPosition({0.0f, 0.0f, 50.0f});
            }

            if(Keyboard::pressed(Keyboard::Key::F2)) {
                const auto projection = glm::perspective(glm::radians(45.0f),
                                                         static_cast<float>(glem::Application::instance().window().width()) / static_cast<float>(glem::Application::instance().window().height()),
                                                         0.1f,
                                                         1000.0f);

                camera_.reset(new MayaCamera{});
                camera_->setProjection(projection);
                camera_->setPosition({0.0f, 0.0f, 50.0f});
            }

            emitter_->spawn();
            emitter_->update(deltaTime);

            camera_->update(deltaTime);

            static const auto speed = 10.0f;

            if(Keyboard::pressed(Keyboard::Key::Up))
                lightPosition_.y += speed * deltaTime;
            if(Keyboard::pressed(Keyboard::Key::Down))
                lightPosition_.y -= speed * deltaTime;
            if(Keyboard::pressed(Keyboard::Key::Right))
                lightPosition_.x += speed * deltaTime;
            if(Keyboard::pressed(Keyboard::Key::Left))
                lightPosition_.x -= speed * deltaTime;
            if(Keyboard::pressed(Keyboard::Key::PageUp))
                lightPosition_.z += speed * deltaTime;
            if(Keyboard::pressed(Keyboard::Key::PageDown))
                lightPosition_.z -= speed * deltaTime;
            if(Keyboard::pressed(Keyboard::Key::Space))
                lightPosition_ = glm::vec3{0.0f};
        }
    }

    void ParticleScene::render() noexcept
    {
        if(visible()) {
            /**** particle rendering ****/
            modelProgram_->bind();
            modelProgram_->setUniform("uProjectionMatrix", camera_->projection());
            modelProgram_->setUniform("uViewMatrix",       camera_->view());
            modelProgram_->setUniform("uViewPosition",     camera_->position());
            modelProgram_->setUniform("uLight.position",   lightPosition_);

            Application::instance().context().beginFrame({0.1f, 0.1f, 0.1f, 1.0f});

            modelVertexArray_->bind();

            emitter_->render(Application::instance().context(), modelProgram_, modelVertexArray_, GL_TRIANGLES);

            lightProgram_->bind();
            lightProgram_->setUniform("uProjectionMatrix", camera_->projection());
            lightProgram_->setUniform("uViewMatrix",       camera_->view());
            lightProgram_->setUniform("uModelMatrix",      glm::translate(glm::mat4{1.0f}, lightPosition_));

            lightVertexArray_->bind();

            Application::instance().context().renderIndexed(lightVertexArray_->indexCount(), GL_TRIANGLE_STRIP);

            Application::instance().context().endFrame();
        }
    }

    /**** TextureMapScene ****/
    TextureMapScene::TextureMapScene()
    {
        attach();
    }

    TextureMapScene::~TextureMapScene()
    {
        detach();
    }

    void TextureMapScene::attach() noexcept
    {
        const auto& vs = R"glsl(
                         #version 450
                         layout(location = 0) in vec3 vPosition;
                         layout(location = 1) in vec3 vNormal;
                            uniform mat4 uProjectionMatrix;
                            uniform mat4 uViewMatrix;
                            uniform mat4 uModelMatrix = mat4(1.0f);
                         void main() {
                            gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(vPosition, 1.0f);
                         }
                         )glsl";

        const auto& ps = R"glsl(
                         #version 450
                         layout(location = 0) out vec4 FragmentColor;
                            uniform vec4 uColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
                         void main() {
                            FragmentColor = uColor;
                         }
                         )glsl";

        lightProgram_ = std::make_shared<Program>();
        lightProgram_->append(std::make_unique<Shader>(vs, ShaderType::VS));
        lightProgram_->append(std::make_unique<Shader>(ps, ShaderType::PS));

        if(!lightProgram_->link()) {
            Log::e(TAG, "Failed to link light shader program.");
            return;
        }

        const auto& phong_textured_vs = R"glsl(
                         #version 450

                         layout(location = 0) in vec3 vPosition;
                         layout(location = 1) in vec3 vNormal;
                         layout(location = 2) in vec2 vUv;

                         uniform mat4 uProjectionMatrix;
                         uniform mat4 uViewMatrix;
                         uniform mat4 uModelMatrix = mat4(1.0f);

                         out vec3 fPosition;
                         out vec3 fNormal;
                         out vec2 fUv;

                         void main() {
                            gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(vPosition, 1.0f);

                            fPosition = vec3(uModelMatrix * vec4(vPosition, 1.0f));
                            fNormal   = mat3(transpose(inverse(uModelMatrix))) * vNormal;
                            fUv       = vUv;
                         }

                         )glsl";

        const auto& phong_textured_ps = R"glsl(
                         #version 450

                         layout(location = 0) out vec4 FragmentColor;

                         uniform vec3 uViewPosition;

                         struct Material {
                            sampler2D diffuse;
                            sampler2D specular;

                            float shininess;
                         };

                         struct Light {
                            vec3 position;

                            vec3 ambient;
                            vec3 diffuse;
                            vec3 specular;

                            float constant;
                            float linear;
                            float quadratic;
                         };

                         uniform Light uLight;
                         uniform Material uMaterial;

                         in vec3 fPosition;
                         in vec3 fNormal;
                         in vec2 fUv;

                         void main() {
                            vec3 ambient = uLight.ambient * vec3(texture2D(uMaterial.diffuse, fUv));

                            vec3 N = normalize(fNormal);
                            vec3 L = normalize(uLight.position - fPosition);

                            vec3 diffuse = uLight.diffuse * max(dot(N, L), 0.0f) * vec3(texture2D(uMaterial.diffuse, fUv));

                            vec3 V = normalize(uViewPosition - fPosition);
                            vec3 R = reflect(-L, N);

                            vec3 specular = uLight.specular * pow(max(dot(V, R), 0.0f), uMaterial.shininess) * vec3(texture2D(uMaterial.specular, fUv));

                            float D = length(uLight.position - fPosition);

                            float attenuation = 1.0f / (uLight.constant + uLight.linear * D + uLight.quadratic * D * D);

                            ambient  *= attenuation;
                            diffuse  *= attenuation;
                            specular *= attenuation;

                            FragmentColor = vec4((ambient + diffuse + specular), 1.0f);
                         }
                         )glsl";

        modelProgram_ = std::make_shared<Program>();
        modelProgram_->append(std::make_unique<Shader>(phong_textured_vs, ShaderType::VS));
        modelProgram_->append(std::make_unique<Shader>(phong_textured_ps, ShaderType::PS));

        if(!modelProgram_->link()) {
            Log::e(TAG, "Failed to link model shader program.");
            return;
        }

        TextureSettings diffuseMapSettings;

        diffuseMapSettings.unit           = 0;
        diffuseMapSettings.usage          = TextureUsage::Texture2D;
        diffuseMapSettings.format         = TextureFormat::RGBA;
        diffuseMapSettings.internalFormat = TextureFormat::RGBA;
        diffuseMapSettings.minFilter      = TextureFilter::Linear;
        diffuseMapSettings.magFilter      = TextureFilter::Linear;
        diffuseMapSettings.wrapSMode      = TextureWrap::ClampToEdge;
        diffuseMapSettings.wrapTMode      = TextureWrap::ClampToEdge;

        diffuseMap_ = std::make_unique<Texture>(*Image::load("container2.png"), diffuseMapSettings);

        TextureSettings specularMapSettings;

        specularMapSettings.unit           = 1;
        specularMapSettings.usage          = TextureUsage::Texture2D;
        specularMapSettings.format         = TextureFormat::RGBA;
        specularMapSettings.internalFormat = TextureFormat::RGBA;
        specularMapSettings.minFilter      = TextureFilter::Linear;
        specularMapSettings.magFilter      = TextureFilter::Linear;
        specularMapSettings.wrapSMode      = TextureWrap::ClampToEdge;
        specularMapSettings.wrapTMode      = TextureWrap::ClampToEdge;

        specularMap_ = std::make_unique<Texture>(*Image::load("container2_specular.png"), specularMapSettings);

        modelProgram_->bind();

        /**** material setup ****/
        modelProgram_->setUniform("uMaterial.diffuse",   static_cast<int>(diffuseMap_->settings().unit));
        modelProgram_->setUniform("uMaterial.specular",  static_cast<int>(specularMap_->settings().unit));
        modelProgram_->setUniform("uMaterial.shininess", 64.0f);

        /**** light setup ****/
        modelProgram_->setUniform("uLight.position",  lightPosition_);
        modelProgram_->setUniform("uLight.ambient",   glm::vec3{0.2f, 0.2f, 0.2f});
        modelProgram_->setUniform("uLight.diffuse",   glm::vec3{0.5f, 0.5f, 0.5f});
        modelProgram_->setUniform("uLight.specular",  glm::vec3{1.0f, 1.0f, 1.0f});
        modelProgram_->setUniform("uLight.constant",  1.0f);
        modelProgram_->setUniform("uLight.linear",    0.09f);
        modelProgram_->setUniform("uLight.quadratic", 0.032f);

        const auto projection = glm::perspective(glm::radians(45.0f),
                                                 static_cast<float>(glem::Application::instance().window().width()) / static_cast<float>(glem::Application::instance().window().height()),
                                                 0.1f,
                                                 1000.0f);

        camera_ = std::make_unique<FreeCamera>();
        camera_->setPosition({0.0f, 0.0f, 5.0f});
        camera_->setProjection(projection);

        /**** init vertex arrays ****/
        auto cube = Shape::texturedCube();

        cube.setFlat();

        modelVertexArray_ = std::make_shared<VertexArray>();
        modelVertexArray_->append(std::make_unique<VertexBuffer>(cube.buffer));
        modelVertexArray_->append(std::make_unique<IndexBuffer>(cube.indices));

        auto sphere = Shape::sphere({12, 12});

        sphere.transform(glm::scale(glm::mat4{1.0f}, glm::vec3{0.2f}));

        InputLayout sphereVertexLayout;
        sphereVertexLayout.push<ElementType::Vector3f>("position");
        sphereVertexLayout.push<ElementType::Vector3f>("normal");

        lightVertexArray_ = std::make_shared<VertexArray>();
        lightVertexArray_->append(std::make_unique<VertexBuffer>(sphere.buffer));
        lightVertexArray_->append(std::make_unique<IndexBuffer>(sphere.indices));

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        setVisible(true);
    }

    void TextureMapScene::detach() noexcept
    {
        setVisible(false);
    }

    void TextureMapScene::update(float deltaTime) noexcept
    {
        if(visible()) {
            if(Keyboard::pressed(Keyboard::Key::F1)) {
                const auto projection = glm::perspective(glm::radians(45.0f),
                                                         static_cast<float>(glem::Application::instance().window().width()) / static_cast<float>(glem::Application::instance().window().height()),
                                                         0.1f,
                                                         1000.0f);

                camera_.reset(new FreeCamera{});
                camera_->setProjection(projection);
                camera_->setPosition({0.0f, 0.0f, 5.0f});
            }

            camera_->update(deltaTime);

            static const auto speed = 10.0f;

            if(Keyboard::pressed(Keyboard::Key::Up))
                lightPosition_.y += speed * deltaTime;
            if(Keyboard::pressed(Keyboard::Key::Down))
                lightPosition_.y -= speed * deltaTime;
            if(Keyboard::pressed(Keyboard::Key::Right))
                lightPosition_.x += speed * deltaTime;
            if(Keyboard::pressed(Keyboard::Key::Left))
                lightPosition_.x -= speed * deltaTime;
            if(Keyboard::pressed(Keyboard::Key::PageUp))
                lightPosition_.z += speed * deltaTime;
            if(Keyboard::pressed(Keyboard::Key::PageDown))
                lightPosition_.z -= speed * deltaTime;
            if(Keyboard::pressed(Keyboard::Key::Space))
                lightPosition_ = glm::vec3{0.0f};
        }
    }

    void TextureMapScene::render() noexcept
    {
        if(visible()) {
            Application::instance().context().beginFrame({0.1f, 0.1f, 0.1f, 1.0f});

            modelProgram_->bind();
            modelProgram_->setUniform("uProjectionMatrix", camera_->projection());
            modelProgram_->setUniform("uViewMatrix",       camera_->view());
            modelProgram_->setUniform("uViewPosition",     camera_->position());
            modelProgram_->setUniform("uLight.position",   lightPosition_);

            diffuseMap_->bind();
            specularMap_->bind();

            modelVertexArray_->bind();

            Application::instance().context().renderIndexed(modelVertexArray_->indexCount(), GL_TRIANGLES);

            lightProgram_->bind();
            lightProgram_->setUniform("uProjectionMatrix", camera_->projection());
            lightProgram_->setUniform("uViewMatrix",       camera_->view());
            lightProgram_->setUniform("uModelMatrix",      glm::translate(glm::mat4{1.0f}, lightPosition_));

            lightVertexArray_->bind();

            Application::instance().context().renderIndexed(lightVertexArray_->indexCount(), GL_TRIANGLE_STRIP);

            Application::instance().context().endFrame();
        }
    }

    /**** SkyboxScene ****/
    SkyboxScene::SkyboxScene()
    {
        attach();
    }

    SkyboxScene::~SkyboxScene()
    {
        detach();
    }

    void SkyboxScene::attach() noexcept
    {
        const auto& vs = R"glsl(
                         #version 450
                         layout(location = 0) in vec3 vPosition;
                            uniform mat4 uProjectionMatrix;
                            uniform mat4 uViewMatrix;
                            out vec3 fUv;
                         void main() {
                            gl_Position = uProjectionMatrix * uViewMatrix * vec4(vPosition, 1.0f);
                            fUv         = vPosition;
                         }
                         )glsl";

        const auto& ps = R"glsl(
                         #version 450
                         layout(location = 0) out vec4 FragmentColor;
                            in vec3 fUv;
                            uniform samplerCube skybox;
                         void main() {
                            FragmentColor = texture(skybox, fUv);
                         }
                         )glsl";

        program_ = std::make_shared<Program>();
        program_->append(std::make_unique<Shader>(vs, ShaderType::VS));
        program_->append(std::make_unique<Shader>(ps, ShaderType::PS));

        if(!program_->link()) {
            Log::e(TAG, "Failed to link skybox shader program.");
            return;
        }

        const auto projection = glm::perspective(glm::radians(45.0f),
                                                 static_cast<float>(glem::Application::instance().window().width()) / static_cast<float>(glem::Application::instance().window().height()),
                                                 0.1f,
                                                 1000.0f);

        camera_ = std::make_unique<FreeCamera>();
        camera_->setPosition({0.0f, 0.0f, 0.0f});
        camera_->setProjection(projection);

        std::array<const char*, 6> faces {
            "skybox/posx.jpg",
            "skybox/negx.jpg",
            "skybox/posy.jpg",
            "skybox/negy.jpg",
            "skybox/posz.jpg",
            "skybox/negz.jpg"
        };

        std::array<Image, 6> images;

        for(size_t i = 0; i < faces.size(); ++i)
            // TODO: replace with if statement aka std::optional
            images[i] = *(Image::load(faces[i], false));

        TextureSettings cubeMapSettings;

        cubeMapSettings.unit           = 0;
        cubeMapSettings.usage          = TextureUsage::TextureCubemap;
        cubeMapSettings.format         = TextureFormat::RGB;
        cubeMapSettings.internalFormat = TextureFormat::RGB;
        cubeMapSettings.minFilter      = TextureFilter::Linear;
        cubeMapSettings.magFilter      = TextureFilter::Linear;
        cubeMapSettings.wrapSMode      = TextureWrap::ClampToEdge;
        cubeMapSettings.wrapTMode      = TextureWrap::ClampToEdge;
        cubeMapSettings.wrapRMode      = TextureWrap::ClampToEdge;

        cubemap_ = std::make_unique<Cubemap>(images, cubeMapSettings);

        auto cube = Shape::cube();

        vertexArray_ = std::make_shared<VertexArray>();
        vertexArray_->append(std::make_unique<VertexBuffer>(cube.buffer));
        vertexArray_->append(std::make_unique<IndexBuffer>(cube.indices));

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        setVisible(true);
    }

    void SkyboxScene::detach() noexcept
    {
        setVisible(false);
    }

    void SkyboxScene::update(float deltaTime) noexcept
    {
        if(visible())
            camera_->update(deltaTime);
    }

    void SkyboxScene::render() noexcept
    {
        if(visible()) {
            Application::instance().context().beginFrame({0.1f, 0.1f, 0.1f, 1.0f});

            glDepthFunc(GL_LEQUAL);

            program_->bind();
            program_->setUniform("uProjectionMatrix", camera_->projection());
            program_->setUniform("uViewMatrix",       glm::mat4{glm::mat3{camera_->view()}});

            cubemap_->bind();
            vertexArray_->bind();

            Application::instance().context().renderIndexed(vertexArray_->indexCount(), GL_TRIANGLES);

            glDepthFunc(GL_LESS);

            Application::instance().context().endFrame();
        }
    }

    DynamicVertexSystemTest::DynamicVertexSystemTest()
    {
        attach();
    }

    DynamicVertexSystemTest::~DynamicVertexSystemTest()
    {
        detach();
    }

    void DynamicVertexSystemTest::attach() noexcept
    {
        const auto& vs = R"glsl(
                         #version 450
                         layout(location = 0) in vec3 v_position;
                         layout(location = 1) in vec3 v_normal;
                            uniform mat4 u_projection;
                            uniform mat4 u_view;
                            uniform mat4 u_model;
                            out vec3 f_normal;
                         void main() {
                            gl_Position = u_projection * u_view * u_model * vec4(v_position, 1.0f);
                            f_normal = v_normal;
                         }
                         )glsl";

        const auto& ps = R"glsl(
                         #version 450
                         layout(location = 0) out vec4 p_color;
                            in vec3 f_normal;
                         void main() {
                            p_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
                         }
                         )glsl";

        program_ = std::make_shared<Program>();

        program_->append(std::make_unique<Shader>(vs, ShaderType::VS));
        program_->append(std::make_unique<Shader>(ps, ShaderType::PS));

        if(!program_->link()) {
            Log::e(TAG, "Failed to link skybox shader program.");
            return;
        }

        const auto projection = glm::perspective(glm::radians(45.0f),
                                                 static_cast<float>(glem::Application::instance().window().width()) / static_cast<float>(glem::Application::instance().window().height()),
                                                 0.1f,
                                                 1000.0f);

        auto camera1 = std::make_unique<FreeCamera>();
        camera1->setProjection(projection);
        camera1->setPosition({0.0f, 0.0f, 10.0f});

        auto camera2 = std::make_unique<MayaCamera>();
        camera2->setProjection(projection);
        camera2->setPosition({0.0f, 0.0f, 10.0f});

        camera_.push_back(std::move(camera1));
        camera_.push_back(std::move(camera2));

        auto model = Shape::sphere({24,12});

        model_ = std::make_shared<VertexArray>();
        model_->append(std::make_unique<VertexBuffer>(model.buffer));
        model_->append(std::make_unique<IndexBuffer>(model.indices));

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        setVisible(true);
    }

    void DynamicVertexSystemTest::detach() noexcept
    {
        setVisible(false);
    }

    void DynamicVertexSystemTest::update(float deltaTime) noexcept
    {
        if(visible()) {
            if(Keyboard::pressed(Keyboard::Key::KP1))
                currentIndex_ = 0;
            if(Keyboard::pressed(Keyboard::Key::KP2))
                currentIndex_ = 1;

            camera_[currentIndex_]->update(deltaTime);
        }
    }

    void DynamicVertexSystemTest::render() noexcept
    {
        if(visible()) {
            program_->bind();
            program_->setUniform("u_projection", camera_[currentIndex_]->projection());
            program_->setUniform("u_view",       camera_[currentIndex_]->view());
            program_->setUniform("u_model",      glm::mat4{1.0f});

            model_->bind();

            Application::instance().context().beginFrame({0.1f, 0.1f, 0.1f, 1.0f});
            Application::instance().context().renderIndexed(model_->indexCount(), GL_TRIANGLE_STRIP);
            Application::instance().context().endFrame();
        }
    }

}
