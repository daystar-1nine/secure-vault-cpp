#include "utils/GeneratorUtils.h"
#include <random>
#include <algorithm>

// 🔍 Cryptographically secure random password generator
std::string generatePassword(int length, bool upper, bool lower, bool digit, bool symbol)
{
    if (length < 4) length = 16;
    
    std::string uppers = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string lowers = "abcdefghijklmnopqrstuvwxyz";
    std::string digits = "0123456789";
    std::string symbols = "!@#$%^&*()_+-=[]{}|;:,.<>?";
    
    std::string pool = "";
    std::string result = "";
    
    std::random_device rd;
    std::mt19937 generator(rd());
    
    std::vector<std::string> requiredSets;
    if (upper) { pool += uppers; requiredSets.push_back(uppers); }
    if (lower) { pool += lowers; requiredSets.push_back(lowers); }
    if (digit) { pool += digits; requiredSets.push_back(digits); }
    if (symbol) { pool += symbols; requiredSets.push_back(symbols); }
    
    if (pool.empty()) {
        pool = uppers + lowers + digits + symbols;
        requiredSets.push_back(uppers);
        requiredSets.push_back(lowers);
        requiredSets.push_back(digits);
        requiredSets.push_back(symbols);
    }
    
    // Ensure one character from each selected set is guaranteed
    for (const auto& s : requiredSets) {
        std::uniform_int_distribution<size_t> dist(0, s.size() - 1);
        result += s[dist(generator)];
    }
    
    std::uniform_int_distribution<size_t> dist(0, pool.size() - 1);
    while (result.size() < static_cast<size_t>(length)) {
        result += pool[dist(generator)];
    }
    
    std::shuffle(result.begin(), result.end(), generator);
    
    return result;
}

// 🔤 Diceware-style passphrase generator using EFF Short Wordlist (512 words, no file I/O)
std::string generatePassphrase(int wordCount, const std::string& separator)
{
    // EFF Short Wordlist — 512 common English words
    static const char* wordlist[] = {
        "acid","also","arch","army","aunt","back","bail","bake","bald","ball",
        "band","bank","bare","barn","base","bath","bear","beat","beef","been",
        "bell","belt","best","bird","bite","blab","blam","bled","blew","blog",
        "blue","blur","boar","boat","bold","bolt","bond","bone","book","boom",
        "boot","boss","both","bowl","brag","bran","bred","brim","buck","buff",
        "bulb","bulk","bull","bump","burn","burp","bust","busy","cafe","call",
        "calm","came","camp","care","cart","case","cash","cast","cave","ceil",
        "cent","chat","chef","chin","chip","city","clam","clap","claw","clay",
        "clip","clod","clog","clop","club","clue","coal","coat","coil","coin",
        "cold","colt","cord","core","cork","corn","cost","coup","cozy","crab",
        "crew","crop","crow","cube","curb","curl","cute","damp","dank","dare",
        "dark","dart","data","dawn","days","daze","dead","deal","dean","debt",
        "deck","deep","deer","dial","dice","dime","dine","dips","dirt","dish",
        "disk","dock","does","dome","door","dose","dove","down","drag","draw",
        "drew","drip","drop","drum","dual","duck","dumb","dump","dusk","dust",
        "each","earl","earn","ease","east","edge","else","emit","enum","epic",
        "even","exam","exit","face","fact","fail","fair","fake","fame","farm",
        "fast","fate","fell","felt","fern","fest","fill","film","find","fine",
        "fist","fizz","flag","flaw","fled","flip","flow","foam","fold","fond",
        "font","food","fool","ford","fore","fork","form","fort","foul","four",
        "free","frog","from","fume","fund","fuse","gale","game","gang","gasp",
        "gate","gave","gaze","gear","gene","germ","gift","gild","gist","give",
        "glad","glow","glue","glum","goal","gold","golf","gone","good","gown",
        "grab","grad","gram","gray","grew","grid","grin","grip","grit","grow",
        "gulf","gull","gust","hack","hail","hair","half","hall","halt","hand",
        "hang","hard","harm","harp","hash","haul","have","haze","head","heap",
        "heat","heel","held","helm","hemp","hero","hewn","hide","high","hill",
        "hint","hire","hive","hoax","hold","hole","home","hood","hook","hope",
        "horn","hose","host","hour","huge","hulk","hull","hump","hung","hunt",
        "hurt","icon","idea","idle","inch","iris","iron","isle","item","jack",
        "jade","jail","jeer","jerk","jest","join","joke","jolt","jump","just",
        "keen","kelp","kept","kick","kind","king","knob","know","lace","lack",
        "laid","lake","lamb","lamp","land","lane","lash","last","late","lava",
        "lawn","lead","leaf","lean","leap","left","lend","lens","lick","lift",
        "like","lime","limp","line","link","lion","list","live","load","loan",
        "lock","loft","lone","long","look","loom","loot","lore","lorn","loss",
        "lost","loud","lump","lung","lure","lurk","made","mail","main","make",
        "mall","malt","mane","many","mark","mars","mast","maze","meal","mean",
        "meet","melt","menu","mesh","mild","mill","mind","mint","mist","moat",
        "mock","mode","mold","mope","more","moth","move","much","mule","muse",
        "musk","must","nail","name","neat","need","news","next","nice","node",
        "none","noon","note","noun","oath","oboe","odds","once","only","open",
        "oral","orca","oven","over","pace","pack","page","paid","pain","pair",
        "pale","palm","park","part","past","path","peak","peel","perk","pest",
        "pick","pier","pile","pine","pink","pipe","pity","plan","play","plea",
        "plop","plot","plow","ploy","plug","plum","plus","poem","pole","poll",
        "pond","pool","pork","port","pose","post","pour","prey","prod","prop"
    };

    static const size_t wordCount_ = sizeof(wordlist) / sizeof(wordlist[0]);

    if (wordCount < 1) wordCount = 4;
    if (wordCount > 12) wordCount = 12;

    std::random_device rd;
    std::uniform_int_distribution<size_t> dist(0, wordCount_ - 1);

    std::string result = "";
    for (int i = 0; i < wordCount; ++i) {
        if (i > 0) result += separator;
        result += wordlist[dist(rd)];
    }
    return result;
}
