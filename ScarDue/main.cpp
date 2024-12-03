#include "components.cpp"

int main() {
    Scene scene(800, 600, "Game Engine");

    World world;
    Entity* player = world.CreateEntity("Player");
    player->AddComponent(std::make_unique<TransformComponent>());
    player->AddComponent(std::make_unique<MeshRendererComponent>());

    while (!scene.ShouldClose()) {
        scene.PollEvents();

        Renderer::Clear();              // Clear the screen
        PhysicsSystem::Update(world.entities, 1.0f / 60.0f); // Physics update
        Renderer::Render(world.entities); // Render entities

        scene.SwapBuffers();            // Swap buffers
    }

    return 0;
}
