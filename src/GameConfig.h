#ifndef CONFIG_H
#define CONFIG_H

struct GameConfig {
    char* filename;

    // Map
    char borderChar;
    int hudHeight;
    
    // Quotas
    int initialScoreQuota;
    int scoreQuotaMultiplier;
    int playerMaxHP;
    int starQuota;
    
    // Timers
    float hunterSpawnInterval;
    float starSpawnInterval;
    float taxiSpawnInterval;
    int endTime;
    
    // Hunters
    int initialHunterCount;
    float hunterSpeed;
    int hunterInitialBounces;
    int hunterDamage;
    char hunterTemplate;
    
    // Seed
    unsigned int seed;

    // Others
    int speedBound;
};

void loadConfig(struct GameConfig* config);

#endif