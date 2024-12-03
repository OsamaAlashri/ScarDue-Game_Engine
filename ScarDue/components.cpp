#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>



// this class includes all game engine components
class Component {
public:
    virtual ~Component() = default;
    virtual void Deserialize(const std::string& data) = 0; 
};

// transform component
class TransformComponent : public Component {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    TransformComponent() : position(0.0f), rotation(0.0f), scale(1.0f) {}

    void Deserialize(const std::string& data) override {
        // Parse position, rotation, and scale from "data" 
        std::cout << "Deserializing TransformComponent from: " << data << std::endl;
    }
};

// mesh Renderer Component
class MeshRendererComponent : public Component {
public:
    std::string meshPath;   
    std::string materialPath; 

    void Deserialize(const std::string& data) override {
        // parsing mesh and material paths from "data"
        std::cout << "Deserializing MeshRendererComponent from: " << data << std::endl;
    }
};

// camera components
class CameraComponent : public Component {
public:
    float fov;
    float nearPlane;
    float farPlane;

    CameraComponent() : fov(45.0f), nearPlane(0.1f), farPlane(100.0f) {}

    void Deserialize(const std::string& data) override {
        // parses the camera attributes from "data"
        std::cout << "Deserializing CameraComponent from: " << data << std::endl;
    }
};

// entity class
class Entity {
public:
    std::string name;
    std::vector<std::unique_ptr<Component>> components;

    void AddComponent(std::unique_ptr<Component> component) {
        components.push_back(std::move(component));
    }

    template<typename T>
    T* GetComponent() {
        for (auto& component : components) {
            if (T* casted = dynamic_cast<T*>(component.get())) {
                return casted;
            }
        }
        return nullptr;
    }
};

// world class 
class World {
public:
    std::vector<std::unique_ptr<Entity>> entities;

    Entity* CreateEntity(const std::string& name) {
        auto entity = std::make_unique<Entity>();
        entity->name = name;
        entities.push_back(std::move(entity));
        return entities.back().get();
    }
};
class Serializer {
public:
    static std::string SerializeEntity(Entity* entity) {
        // serialize entity name and components
        std::string data = "Entity: " + entity->name + "\n";
        for (const auto& component : entity->components) {
            // serialize each component 
            data += "Component Data\n";
        }
        return data;
    }

    static void DeserializeEntity(Entity* entity, const std::string& data) {
        // parse data to populate entity and components
        std::cout << "Deserializing Entity: " << data << std::endl;
    }
};
class Event {
public:
    virtual ~Event() = default;
    virtual void Handle() = 0;
};

class InputEvent : public Event {
public:
    std::string key;
    bool pressed;

    InputEvent(const std::string& key, bool pressed) : key(key), pressed(pressed) {}

    void Handle() override {
        std::cout << (pressed ? "Key Pressed: " : "Key Released: ") << key << std::endl;
    }
};
// renderer class
class Renderer {
public:
    static void Clear() {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    static void Render(const std::vector<std::unique_ptr<Entity>>& entities) {
        for (const auto& entity : entities) {
            if (auto meshRenderer = entity->GetComponent<MeshRendererComponent>()) {
                // Rendering logic here (mock for now)
                std::cout << "Rendering Mesh: " << meshRenderer->meshPath << std::endl;
            }
        }
    }
};

class PhysicsSystem {
public:
    static void Update(std::vector<std::unique_ptr<Entity>>& entities, float deltaTime) {
        for (auto& entity : entities) {
            if (auto transform = entity->GetComponent<TransformComponent>()) {
                // Apply simple physics logic
                transform->position += glm::vec3(0.0f, -9.8f * deltaTime, 0.0f); // Gravity example
                std::cout << "Updated Position: " << transform->position.x << ", " << transform->position.y << ", " << transform->position.z << std::endl;
            }
        }
    }
};
class Scene {

public:
    GLFWwindow* window;

    Scene(int width, int height, const std::string& title) {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW!" << std::endl;
            exit(-1);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window!" << std::endl;
            glfwTerminate();
            exit(-1);
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD!" << std::endl;
            exit(-1);
        }

        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST); // Enable depth testing
    }

    ~Scene() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool ShouldClose() const {
        return glfwWindowShouldClose(window);
    }

    void SwapBuffers() const {
        glfwSwapBuffers(window);
    }

    void PollEvents() const {
        glfwPollEvents();
    }
};



