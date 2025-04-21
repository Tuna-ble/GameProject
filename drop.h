#ifndef DROP_H_INCLUDED
#define DROP_H_INCLUDED

enum class dropType {
    SPEED,
    DAMAGE,
    HEALTH
};

//struct Drop {
//    Vector2D position;
//    SDL_Texture* texture;
//    SDL_Rect dest;
//    SDL_Rect srcRect;
//    DropType type;
//
//    int buffValue;
//    bool active;
//    bool pickedUp = false;
//
//    float activeTimer = 0.0f;
//    float activeDuration = 10.0f;
//
//    Drop(Vector2D position, SDL_Texture* texture)
//    void update(float deltaTime);
//    void render(SDL_Renderer* renderer, const Camera& camera);
//    SDL_Rect getCollider() const;
//};
//
//struct DropManager {
//    std::vector<Drop> drops;
//    SDL_Texture* damageTexture;
//    SDL_Texture* healthTexture;
//    SDL_Texture* speedTexture;
//
//    void spawnDrop(Vector2D position);
//    void update(float deltaTime, const SDL_Rect& playerCollider);
//    void render(SDL_Renderer* renderer, const Camera& camera);
//};

#endif // DROP_H_INCLUDED
