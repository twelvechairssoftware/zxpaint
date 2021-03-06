#include "main.h"

bool initSDL() {
    try {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            spdlog::error(SDL_GetError());
            return false;
        }

        mainWindow = SDL_CreateWindow("ZX-Paint", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      maxScreenWidth, maxScreenHeight, SDL_WINDOW_RESIZABLE);
        if (mainWindow == nullptr){
            spdlog::error(SDL_GetError());
            return false;
        }

        mainRender = SDL_CreateRenderer(mainWindow, 0,  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (mainRender == nullptr) {
            spdlog::error(SDL_GetError());
            return false;
        }
        return true;
    }
    catch (const std::exception&){
        spdlog::error(SDL_GetError());
        return false;
    }
}

void exitSDL(){
    try {
        SDL_DestroyRenderer(mainRender);
        mainRender = nullptr;
        SDL_DestroyWindow(mainWindow);
        mainWindow = nullptr;

        SDL_Quit();
    }
    catch (const std::exception&){
        spdlog::error(SDL_GetError());
    }
}

void drawRightMenuPane(){
    try {
        SDL_Rect fillRect = {0, 0, 130, maxScreenHeight};
        SDL_SetRenderDrawColor(mainRender, 220, 220, 220, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(mainRender, &fillRect);
    }
    catch (std::exception &e){
        spdlog::error("{}, {}", e.what(), SDL_GetError());
    }
}

void drawScreen(){
    try {
        SDL_Rect fillRect;
        int attributeGridSize = pixelSize * 8;

        for (int y = 0; y < attributes[0].size(); y++) {
            for (int x = 0; x < attributes.size(); x++) {
                fillRect = {x * attributeGridSize, y * attributeGridSize, attributeGridSize, attributeGridSize};
                attribute attr = attributes.at(x).at(y);
                auto temp = colorPalette[attr.bright][attr.paper];
                SDL_SetRenderDrawColor(mainRender, temp.r, temp.g, temp.b, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(mainRender, &fillRect);

                int start_y = y * 8;
                int start_x = x * 8;
                for (int pixel_y = start_y; pixel_y <= start_y + 8; pixel_y++) {
                    for (int pixel_x = start_x; pixel_x < start_x + 8; pixel_x++) {
                        fillRect = {pixel_x * pixelSize, pixel_y * pixelSize, pixelSize, pixelSize};
                        if (pixels[pixel_x][pixel_y]) {
                            temp = colorPalette[attr.bright][attr.ink];
                            SDL_SetRenderDrawColor(mainRender, temp.r, temp.g, temp.b, SDL_ALPHA_OPAQUE);
                            SDL_RenderFillRect(mainRender, &fillRect);
                        }
                    }
                }

                SDL_SetRenderDrawColor(mainRender, 20, 20, 20, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawRect(mainRender, &fillRect);
            }
        }
    }
    catch (std::exception &e){
        spdlog::error("{}, {}", e.what(), SDL_GetError());
    }
}

void drawGrid(){
    try {
        if (showGrid) {
            SDL_Rect fillRect;
            // Minor ticks
            for (int y = 0; y < pixels[0].size(); y++) {
                for (int x = 0; x < pixels.size(); x++) {
                    fillRect = {x * pixelSize, y * pixelSize, pixelSize, pixelSize};
                    if (pixelSize >= 4) {
                        SDL_SetRenderDrawBlendMode(mainRender, SDL_BLENDMODE_BLEND);
                        SDL_SetRenderDrawColor(mainRender, 0, 0, 0, 15);
                        SDL_RenderDrawRect(mainRender, &fillRect);
                    }
                }
            }
            // Major ticks
            for (int y = 0; y < attributes[0].size(); y++) {
                for (int x = 0; x < attributes.size(); x++) {
                    fillRect = {x * attributeSize, y * attributeSize, attributeSize, attributeSize};
                    if (pixelSize >= 4) {
                        SDL_SetRenderDrawBlendMode(mainRender, SDL_BLENDMODE_BLEND);
                        SDL_SetRenderDrawColor(mainRender, 0, 0, 0, 30);
                        SDL_RenderDrawRect(mainRender, &fillRect);
                    }
                }
            }
        }
    }
    catch (std::exception &e){
        spdlog::error("{}, {}", e.what(), SDL_GetError());
    }
}

void drawColorOptions(){
    try {
        int allColors = blockSize * colorPalette[0].size();
        int startingPositionX = 30;
        int startingPositionY = (maxScreenHeight - allColors) - 20;
        SDL_Rect fillRect;

        for (int index = 0; index <= 1; index++) {
            int subindex = 0;
            int offset = 0;
            for (auto color: colorPalette[index]) {
                offset = (subindex <= 0) ? 0 : 1;
                fillRect = {
                        (startingPositionX + index) + (index * blockSize),
                        (startingPositionY + (subindex * blockSize)) + offset,
                        blockSize,
                        blockSize - offset
                };
                SDL_SetRenderDrawColor(mainRender, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(mainRender, &fillRect);

                if (colorLocations[index][subindex].hover || colorLocations[index][subindex].selected) {
                    SDL_SetRenderDrawColor(mainRender, 0, 0, 0, SDL_ALPHA_OPAQUE);
                    SDL_RenderDrawRect(mainRender, &fillRect);
                }

                colorLocations.at(index).at(subindex) = {
                        fillRect.x, fillRect.y, fillRect.x + blockSize, fillRect.y + blockSize
                };

                subindex++;
            }
        }

        // Ink color
        fillRect = {
                iconLocations[8].x1 + 1,
                iconLocations[8].y1 + 30,
                blockSize - 2,
                5
        };
        SDL_SetRenderDrawColor(mainRender,
                               colorPalette[selectedColors.bright][selectedColors.ink].r,
                               colorPalette[selectedColors.bright][selectedColors.ink].g,
                               colorPalette[selectedColors.bright][selectedColors.ink].b,
                               SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(mainRender, &fillRect);

        // Paper color
        fillRect = {
                iconLocations[9].x1 + 1,
                iconLocations[9].y1 + 30,
                blockSize - 2,
                5
        };
        SDL_SetRenderDrawColor(mainRender,
                               colorPalette[selectedColors.bright][selectedColors.paper].r,
                               colorPalette[selectedColors.bright][selectedColors.paper].g,
                               colorPalette[selectedColors.bright][selectedColors.paper].b,
                               SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(mainRender, &fillRect);
    }
    catch (std::exception &e){
        spdlog::error("{}, {}", e.what(), SDL_GetError());
    }
}

void mouseEvents(int index){
    try {
        if (mouseLocation.clicked) {
            // Clicked on main board
            if (mouseLocation.x / pixelSize < pixels.size()) {
                auto attr_copy = attributes[(mouseLocation.x / pixelSize) / 8][(mouseLocation.y / pixelSize) / 8];
                if (ink){
                    pixels[mouseLocation.x / pixelSize][mouseLocation.y / pixelSize] = true;
                    attributes[(mouseLocation.x / pixelSize) / 8][(mouseLocation.y / pixelSize) / 8] = {
                            selectedColors.ink,
                            attr_copy.paper,
                            selectedColors.bright
                    };
                }
                else {
                    attributes[(mouseLocation.x / pixelSize) / 8][(mouseLocation.y / pixelSize) / 8] = {
                            attr_copy.ink,
                            selectedColors.paper,
                            attr_copy.bright
                    };
                }
            }
                // Clicked menu bar
            else {
                // Check icons
                if ((iconLocations[index].x1 <= mouseLocation.x && mouseLocation.x <= iconLocations[index].x2) &&
                    (iconLocations[index].y1 <= mouseLocation.y && mouseLocation.y <= iconLocations[index].y2)) {
                    iconLocations[index].selected = true;

                    // Click events
                    switch (index) {
                        case 0:
                            if (pixelSize <= 200) {
                                pixelSize += 1;
                            }
                            break;
                        case 1:
                            if (pixelSize >= 2) {
                                pixelSize -= 1;
                            }
                            break;
                        case 2:
                            showGrid = true;
                            break;
                        case 3:
                            showGrid = false;
                            break;
                        case 8:
                            ink = true;
                            iconLocations[8].selected = true;
                            iconLocations[9].selected = false;
                            break;
                        case 9:
                            ink = false;
                            iconLocations[8].selected = false;
                            iconLocations[9].selected = true;
                            break;
                        default:
                            break;
                    }
                }
                else {
                    iconLocations[index].selected = false;
                }

                // Check colours
                for (int n = 0; n < colorLocations.size(); n++) {
                    for (int m = 0; m < colorLocations[n].size(); m++) {
                        if ((colorLocations[n][m].x1 <= mouseLocation.x && mouseLocation.x <= colorLocations[n][m].x2) &&
                            (colorLocations[n][m].y1 <= mouseLocation.y && mouseLocation.y <= colorLocations[n][m].y2)) {
                            colorLocations[n][m].selected = true;
                            selectedColors.ink = randomInteger(7);
                            selectedColors.paper = randomInteger(7);
                            selectedColors.bright = randomInteger(1);
                        }
                        else {
                            colorLocations[n][m].selected = false;
                        }
                    }
                }
            }
        }
            // Not clicked
        else {
            // Icons
            if ((iconLocations[index].x1 <= mouseLocation.x && mouseLocation.x <= iconLocations[index].x2) &&
                (iconLocations[index].y1 <= mouseLocation.y && mouseLocation.y <= iconLocations[index].y2)) {
                iconLocations[index].hover = true;
            }
            else {
                iconLocations[index].hover = false;
            }

            // Colors
            for (int n = 0; n < 2; n++) {
                for (int m = 0; m < 8; m++) {
                    if ((colorLocations[n][m].x1 <= mouseLocation.x && mouseLocation.x <= colorLocations[n][m].x2) &&
                        (colorLocations[n][m].y1 <= mouseLocation.y && mouseLocation.y <= colorLocations[n][m].y2)) {
                        colorLocations[n][m].hover = true;
                    }
                    else {
                        colorLocations[n][m].hover = false;

                    }
                }
            }
        }
    }
    catch (std::exception &e){
        spdlog::error("{}, {}", e.what(), SDL_GetError());
    }
}

void drawIcons(){
    try {
        SDL_Rect bitmapLayer;
        SDL_Rect outlineRect;

        int index = 0;
        int startingPositionX;
        int startingPositionY;

        for (SDL_Texture* texture: textures) {
            startingPositionX = 30;
            startingPositionY = 20 + (icons.size() / sizeof(icons));
            if (index % 2 == 0) {
                bitmapLayer = {startingPositionX, (index * blockSize) + startingPositionY, blockSize, blockSize};
                outlineRect = {startingPositionX, (index * blockSize) + startingPositionY, blockSize, blockSize};

                iconLocations[index].x1 = startingPositionX;
                iconLocations[index].x2 = startingPositionX + blockSize;
            } else {
                startingPositionY -= blockSize;
                bitmapLayer = {startingPositionX + blockSize + 1, (index * blockSize) + startingPositionY, blockSize,
                               blockSize};
                outlineRect = {startingPositionX + blockSize + 1, (index * blockSize) + startingPositionY, blockSize,
                               blockSize};

                iconLocations[index].x1 = startingPositionX + blockSize + 1;
                iconLocations[index].x2 = (startingPositionX + blockSize + 1) + blockSize;
            }

            iconLocations[index].y1 = (index * blockSize) + startingPositionY;
            iconLocations[index].y2 = ((index * blockSize) + startingPositionY) + blockSize;

            // Icon background color
            SDL_SetRenderDrawColor(mainRender, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(mainRender, &outlineRect);

            SDL_RenderCopy(mainRender, texture, nullptr, &bitmapLayer);

            if (iconLocations[index].hover || iconLocations[index].selected) {
                SDL_SetRenderDrawColor(mainRender, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawRect(mainRender, &outlineRect);
            }

            index++;
        }
    }
    catch (std::exception &e){
        spdlog::error("{}, {}", e.what(), SDL_GetError());
    }
}

void preLoadImages(){
    try {
        for (auto &icon: icons) {
            SDL_Surface *surface = IMG_ReadXPMFromArray((char **) icon);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(mainRender, surface);
            if (texture == nullptr) {
                spdlog::error("Unable to load image: {}\n", SDL_GetError());
            }
            SDL_FreeSurface(surface);
            textures.push_back(texture);
        }
    }
    catch (std::exception &e){
        spdlog::error("{}, {}", e.what(), SDL_GetError());
    }
}

void redrawMenu(){
    SDL_RenderSetViewport(mainRender, &mainMenu);
    drawRightMenuPane();
    drawColorOptions();
    drawIcons();
}

void redrawMainScreen(){
    SDL_RenderSetViewport(mainRender, &mainGrid);
    drawScreen();
    drawGrid();
}

void windowResized(){
    mainGrid = {0, 0, maxScreenWidth - 130, maxScreenHeight};
    mainMenu = {maxScreenWidth - 129, 0, 130, maxScreenHeight};
}

void redraw(){
    SDL_SetRenderDrawColor(mainRender, 255,255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(mainRender);
    redrawMainScreen();
    redrawMenu();
    SDL_RenderPresent(mainRender);
}


void reset(){
    ink = true;

    mainGrid = {0, 0, maxScreenWidth - 130, maxScreenHeight};
    mainMenu = {maxScreenWidth - 129, 0, 130, maxScreenHeight};

    // 256x192 (1x1) pixels
    pixels = {256, std::vector<bool>(192,false)};
    // 32x24 (8x8) attributes
    attributes = {32, std::vector<attribute>(24, {0, 7, true})};

    selectedColors = {0, 2, true};

    colorPalette = {
            {
                    rgb{0, 0, 0}, //BLACK0
                    rgb{0, 0, 215}, //BLUE0
                    rgb{215, 0, 0}, //RED0
                    rgb{215, 0, 215}, //MAGENTA0
                    rgb{0, 215, 0}, //GREEN0
                    rgb{0, 215, 215}, //CYAN0
                    rgb{215, 215, 0}, //YELLOW0
                    rgb{215, 215, 215} //WHITE0
            },
            {
                    rgb{0, 0, 0}, //BLACK1
                    rgb{0, 0, 255}, //BLUE1
                    rgb{255, 0, 0}, //RED1
                    rgb{255, 0, 255}, //MAGENTA1
                    rgb{0, 255, 0}, //GREEN1
                    rgb{0, 255, 255}, //CYAN1
                    rgb{255, 255, 0}, //YELLOW1
                    rgb{255, 255, 255} //WHITE1
            }
    };

    objectLocation location = {
            0,
            0,
            0,
            0,
            false,
            false
    };

    iconLocations = {10, location};
    iconLocations[8].selected = true;

    colorLocations = {colorPalette.size(), std::vector<objectLocation>(colorPalette[0].size(), location)};

    preLoadImages();

    redraw();
}

int main(int argc, char* args[]){
    try {
        if (!initSDL()) {
            spdlog::error("Error initializing SDL - {}", SDL_GetError());
        }
        else {
            SDL_Event e;
            SDL_ShowCursor(1);

            reset();

            bool mainLoopRunning = true;

            while (mainLoopRunning) {
                while (SDL_PollEvent(&e) != 0) {
                    switch (e.type){
                        case SDL_MOUSEMOTION:
                            SDL_GetMouseState(&mouseLocation.x, &mouseLocation.y);
                            mouseEvents(1);
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            mouseLocation.clicked = true;
                            mouseEvents(0);
                            break;
                        case SDL_MOUSEBUTTONUP:
                            mouseLocation.clicked = false;
                            mouseEvents(2);
                            break;
                        case SDL_WINDOWEVENT:
                            if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                                maxScreenWidth = e.window.data1;
                                maxScreenHeight = e.window.data2;
                                windowResized();
                            }
                            break;
                        case SDL_KEYDOWN:
                            spdlog::debug("key");
                            break;
                        case SDL_QUIT:
                            spdlog::info("SDL exiting");
                            mainLoopRunning = false;
                            break;
                    }
                    redraw();
                }
                SDL_Delay(10);
            }
        }

        exitSDL();
        return 0;
    }
    catch (const std::exception &e){
        spdlog::error("General error: {}\n\n{}", e.what(), SDL_GetError());
        exitSDL();
        return 1;
    }
}
