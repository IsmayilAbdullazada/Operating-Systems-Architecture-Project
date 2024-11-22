import random

english_words = [
    "hello", "goodbye", "cat", "dog", "sun", "moon", "tree", "car", "book", "house", "city", "friend", "family", "love", "happy", "sad", "eat", "drink", "sleep", "walk", "run", "see", "hear", "speak", "learn", "work", "play", "think", "know", "understand", "time", "year", "day", "night", "man", "woman", "child", "world", "school", "student", "teacher", "doctor", "hospital", "money", "food", "water", "air", "fire", "earth", "sky", "color", "number", "name", "question", "answer",
    "table", "chair", "computer", "phone", "window", "door", "room", "building", "street", "road", "mountain", "river", "sea", "country", "language", "word", "sentence", "story", "music", "movie", "art", "science", "history", "politics", "economy", "business", "job", "travel", "vacation", "sport", "game", "team", "player", "win", "lose", "begin", "end", "start", "finish", "wait", "listen", "read", "write", "buy", "sell", "pay", "cost", "price", "cheap", "expensive",
]

french_words = [
    "bonjour", "au revoir", "chat", "chien", "soleil", "lune", "arbre", "voiture", "livre", "maison", "ville", "ami", "famille", "amour", "heureux", "triste", "manger", "boire", "dormir", "marcher", "courir", "voir", "entendre", "parler", "apprendre", "travailler", "jouer", "penser", "savoir", "comprendre", "temps", "an", "jour", "nuit", "homme", "femme", "enfant", "monde", "école", "élève", "professeur", "médecin", "hôpital", "argent", "nourriture", "eau", "air", "feu", "terre", "ciel", "couleur", "nombre", "nom", "question", "réponse",
    "table", "chaise", "ordinateur", "téléphone", "fenêtre", "porte", "pièce", "bâtiment", "rue", "route", "montagne", "rivière", "mer", "pays", "langue", "mot", "phrase", "histoire", "musique", "film", "art", "science", "histoire", "politique", "économie", "affaires", "travail", "voyage", "vacances", "sport", "jeu", "équipe", "joueur", "gagner", "perdre", "commencer", "finir", "commencer", "terminer", "attendre", "écouter", "lire", "écrire", "acheter", "vendre", "payer", "coûter", "prix", "bon marché", "cher",
]

# Add more words related to nature and environment
english_words.extend([
    "forest", "lake", "beach", "flower", "grass", "leaf", "stone", "sand", "rain", "snow", "wind", "storm", "cloud", "weather"
])
french_words.extend([
    "forêt", "lac", "plage", "fleur", "herbe", "feuille", "pierre", "sable", "pluie", "neige", "vent", "tempête", "nuage", "temps"
])

# Add words related to animals
english_words.extend([
    "bird", "fish", "cow", "horse", "sheep", "goat", "rabbit", "lion", "tiger", "bear", "elephant", "snake", "monkey", "chicken"
])
french_words.extend([
    "oiseau", "poisson", "vache", "cheval", "mouton", "chèvre", "lapin", "lion", "tigre", "ours", "éléphant", "serpent", "singe", "poulet"
])

# Add words related to body and health
english_words.extend([
    "head", "face", "eye", "nose", "mouth", "ear", "hand", "arm", "leg", "foot", "heart", "blood", "health", "pain", "sick"
])
french_words.extend([
    "tête", "visage", "œil", "nez", "bouche", "oreille", "main", "bras", "jambe", "pied", "cœur", "sang", "santé", "douleur", "malade"
])

# Add words related to technology and tools
english_words.extend([
    "keyboard", "screen", "mouse", "camera", "television", "radio", "printer", "machine", "tool", "engine", "battery", "electricity"
])
french_words.extend([
    "clavier", "écran", "souris", "caméra", "télévision", "radio", "imprimante", "machine", "outil", "moteur", "batterie", "électricité"
])

# Add words related to food and drink
english_words.extend([
    "bread", "butter", "cheese", "milk", "egg", "fruit", "vegetable", "meat", "fish", "chicken", "soup", "sauce", "coffee", "tea", "juice"
])
french_words.extend([
    "pain", "beurre", "fromage", "lait", "œuf", "fruit", "légume", "viande", "poisson", "poulet", "soupe", "sauce", "café", "thé", "jus"
])

# Add words related to feelings and emotions
english_words.extend([
    "angry", "scared", "excited", "bored", "tired", "confused", "surprised", "nervous", "calm", "confident", "grateful"
])
french_words.extend([
    "en colère", "effrayé", "excité", "ennuyé", "fatigué", "confus", "surpris", "nerveux", "calme", "confiant", "reconnaissant"
])

# Add more action words
english_words.extend([
    "help", "fix", "carry", "push", "pull", "throw", "catch", "climb", "fall", "jump", "turn", "move", "dance", "sing"
])
french_words.extend([
    "aider", "réparer", "porter", "pousser", "tirer", "jeter", "attraper", "grimper", "tomber", "sauter", "tourner", "bouger", "danser", "chanter"
])

# Add travel and places vocabulary
english_words.extend([
    "train", "bus", "station", "airport", "ticket", "passport", "suitcase", "map", "hotel", "restaurant", "museum", "park"
])
french_words.extend([
    "train", "bus", "gare", "aéroport", "billet", "passeport", "valise", "carte", "hôtel", "restaurant", "musée", "parc"
])

# Add adjectives
english_words.extend([
    "big", "small", "fast", "slow", "new", "old", "hot", "cold", "beautiful", "ugly", "easy", "difficult", "strong", "weak"
])
french_words.extend([
    "grand", "petit", "rapide", "lent", "nouveau", "vieux", "chaud", "froid", "beau", "laid", "facile", "difficile", "fort", "faible"
])

with open("dictionary_1000.txt", "w", encoding="utf-8") as f:
    for _ in range(50):
        english = random.choice(english_words)
        french = french_words[english_words.index(english)]
        f.write(f"{english};{french}\n")
