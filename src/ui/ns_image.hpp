
#pragma once

#include "ui.h"
#include "items.hpp"
#include "D3DDevice9.h"

TRef<ConstantImage> LoadImageFile(LuaScriptContext& context, std::string path) {
    std::string fullpath = context.FindPath(path);
    Engine* pEngine = context.GetEngine();

    if (fullpath == "") {
        throw std::runtime_error("Path not found: " + path);
    }

    const char* charString = fullpath.c_str();

    PathString pathString = PathString(ZString(charString));

    TRef<ZFile> zf = new ZFile(pathString, OF_READ | OF_SHARE_DENY_WRITE);
    ZFile * pFile = (ZFile*)zf;

    D3DXIMAGE_INFO fileInfo;
    if (D3DXGetImageInfoFromFileInMemory(pFile->GetPointer(),
        pFile->GetLength(),
        &fileInfo) == D3D_OK)
    {
        _ASSERT(fileInfo.ResourceType == D3DRTYPE_TEXTURE);

        // We can resize non-UI textures.
        WinPoint targetSize(fileInfo.Width, fileInfo.Height);

        // For D3D9, we only allow black colour keys.
        TRef<Surface> psurface =
            pEngine->CreateSurfaceD3DX(
                &fileInfo,
                &targetSize,
                zf,
                false,
                Color(0, 0, 0),
                pathString);

        return new ConstantImage(psurface, pathString);
    }
    else
    {
        throw std::runtime_error("Failed to load image: " + pathString);
    }
}

class ImageNamespace {
public:
    static void AddNamespace(LuaScriptContext& context) {
        sol::table table = context.GetLua().create_table();
        table["Empty"] = []() {
            return (TRef<Image>)Image::GetEmpty();
        };

        table["Lazy"] = [&context](sol::function callback) {
            return ImageTransform::Lazy(context.WrapCallback<TRef<Image>>(callback, Image::GetEmpty()));
        };

        table["Extent"] = sol::overload(
            [](RectValue* rect, ColorValue* color) {
                if (!rect || !color) {
                    throw std::runtime_error("Argument should not be null");
                }
                return CreateExtentImage(rect, color);
            },
            [](PointValue* pPoint, ColorValue* color) {
                if (!pPoint || !color) {
                    throw std::runtime_error("Argument should not be null");
                }

                Number* zero = new Number(0.0f);

                return CreateExtentImage(
                    RectTransform::Create(
                        zero,
                        zero,
                        PointTransform::X(pPoint),
                        PointTransform::Y(pPoint)
                    ),
                    color
                );
            }
        );
        table["MouseEvent"] = [](Image* image) {
            if (!image) {
                throw std::runtime_error("Argument should not be null");
            }
            return (TRef<Image>)new MouseEventImage(image);
        };
        table["File"] = [&context](std::string path) {
            return (TRef<ConstantImage>)LoadImageFile(context, path);
        };
        table["Group"] = [](sol::object list) {
            if (list.is<TRef<ImageList>>()) {
                TRef<ImageList> varlist = list.as<TRef<ImageList>>();

                return (TRef<Image>)new NonStaticCallbackImage<ImageList*>([](ImageList* varlist) {
                    TRef<GroupImage> pgroup = new GroupImage();
                    for (auto entry : varlist->GetList()) {
                        pgroup->AddImageToTop(entry);
                    }
                    return (TRef<Image>)pgroup;
                }, varlist);
            }
            else if (list.is<sol::table>()) {
                TRef<GroupImage> pgroup = new GroupImage();

                sol::table table_list = list;
                int count = table_list.size();

                TRef<Image> child;

                for (int i = 1; i <= count; ++i) {
                    child = table_list.get<Image*>(i);
                    if (!child) {
                        throw std::runtime_error("Element in group should not be null");
                    }
                    pgroup->AddImageToTop(child);
                }

                return (TRef<Image>)pgroup;
            }
            throw std::runtime_error("Expected value argument of Image.Group to be either a table of images or a result from List.MapToImages");
        };
        table["Switch"] = [&context](sol::object value, sol::table table) {
            int count = table.size();

            if (value.is<TStaticValue<ZString>>() || value.is<std::string>()) {
                //the wrapped value is a ZString, the unwrapped value a std::string
                std::map<std::string, TRef<Image>> mapOptions;

                table.for_each([&mapOptions](sol::object key, sol::object value) {
                    std::string strKey = key.as<std::string>();
                    mapOptions[strKey] = value.as<Image*>();
                });

                return ImageTransform::Switch(wrapString(value), mapOptions);
            }
            else if (value.is<Number>() || value.is<float>()) {
                std::map<int, TRef<Image>> mapOptions;

                table.for_each([&mapOptions](sol::object key, sol::object value) {
                    int fKey = (int)key.as<float>();
                    mapOptions[fKey] = value.as<Image*>();
                });

                return ImageTransform::Switch(wrapValue<float>(value), mapOptions);
            }
            else if (value.is<Boolean>() || value.is<bool>()) {
                std::map<bool, TRef<Image>> mapOptions;

                table.for_each([&mapOptions](sol::object key, sol::object value) {
                    bool bKey = key.as<bool>();
                    mapOptions[bKey] = value.as<Image*>();
                });

                return ImageTransform::Switch(wrapValue<bool>(value), mapOptions);
            }
            else if (value.is<UiStateValue>()) {
                typedef std::function<TRef<Image>(UiObjectContainer*)> MapValueType;
                std::map<std::string, sol::function> mapOptions;

                table.for_each([&mapOptions](sol::object key, sol::object value) {
                    std::string strKey = key.as<std::string>();
                    mapOptions[strKey] = value.as<sol::function>();
                });

                return (TRef<Image>)new CallbackImage<UiState>([&context, mapOptions](UiState state) {
                    auto find = mapOptions.find(state.GetName());
                    if (find == mapOptions.end()) {
                        return (TRef<Image>)Image::GetEmpty();
                    }
                    return (TRef<Image>)context.WrapCallback<TRef<Image>, UiState*>(find->second, Image::GetEmpty())(&state);
                }, wrapValue<UiState>(value));
            }
            throw std::runtime_error("Expected value argument of Image.Switch to be either a wrapped or unwrapped bool, int, or string");
        };
        table["String"] = [](FontValue* font, ColorValue* color, sol::object width, sol::object string, sol::optional<Justification> justification_arg) {
            if (!font || !color) {
                throw std::runtime_error("Argument should not be null");
            }

            return ImageTransform::String(font, color, wrapValue<float>(width), wrapString(string), justification_arg.value_or(JustifyLeft()));
        };
        table["Translate"] = [](Image* pimage, PointValue* pPoint) {
            if (!pimage || !pPoint) {
                throw std::runtime_error("Argument should not be null");
            }
            return ImageTransform::Translate(pimage, pPoint);
        };
        table["Scale"] = [](Image* pimage, PointValue* pPoint) {
            if (!pimage || !pPoint) {
                throw std::runtime_error("Argument should not be null");
            }
            return ImageTransform::Scale(pimage, pPoint);
        };
        table["Rotate"] = [](Image* pimage, sol::object radians) {
            if (!pimage) {
                throw std::runtime_error("Argument should not be null");
            }
            return ImageTransform::Rotate(pimage, wrapValue<float>(radians));
        };
        table["Size"] = [](Image* pimage) {
            if (!pimage) {
                throw std::runtime_error("Argument should not be null");
            }
            return ImageTransform::Size(pimage);
        };

        table["Justify"] = [](Image* pimage, PointValue* pSizeContainer, Justification justification) {
            if (!pimage || !pSizeContainer) {
                throw std::runtime_error("Argument should not be null");
            }
            return ImageTransform::Justify(pimage, pSizeContainer, justification);
        };
        table["ScaleFit"] = [](Image* pimage, PointValue* pSizeContainer, Justification justification) {
            if (!pimage || !pSizeContainer) {
                throw std::runtime_error("Argument should not be null");
            }
            return ImageTransform::ScaleFit(pimage, pSizeContainer, justification);
        };
        table["ScaleFill"] = [](Image* pimage, PointValue* pSizeContainer, Justification justification) {
            if (!pimage || !pSizeContainer) {
                throw std::runtime_error("Argument should not be null");
            }
            return ImageTransform::ScaleFill(pimage, pSizeContainer, justification);
        };

        table["Clip"] = [](Image* pimage, RectValue* rect) {
            if (!pimage || !rect) {
                throw std::runtime_error("Argument should not be null");
            }
            return ImageTransform::Clip(pimage, rect);
        };
        table["Cut"] = [](Image* pimage, RectValue* rect) {
            if (!pimage || !rect) {
                throw std::runtime_error("Argument should not be null");
            }
            return ImageTransform::Cut(pimage, rect);
        };

        table["Multiply"] = [](ConstantImage* pimage, ColorValue* color) {
            if (!pimage || !color) {
                throw std::runtime_error("Argument should not be null");
            }
            return ImageTransform::Multiply(pimage, color);
        };

        context.GetLua().new_usertype<ConstantImage>("ConstantImage",
            sol::base_classes, sol::bases<Image>()
        );
        context.GetLua().set("Image", table);

        sol::table tableJustification = context.GetLua().create_table();
        tableJustification["Left"] = (Justification)JustifyLeft();
        tableJustification["Right"] = (Justification)JustifyRight();
        tableJustification["Top"] = (Justification)JustifyTop();
        tableJustification["Bottom"] = (Justification)JustifyBottom();
        tableJustification["Center"] = (Justification)JustifyCenter();
        tableJustification["Topleft"] = (Justification)(JustifyTop() | JustifyLeft());
        tableJustification["Topright"] = (Justification)(JustifyTop() | JustifyRight());
        tableJustification["Bottomleft"] = (Justification)(JustifyBottom() | JustifyLeft());
        tableJustification["Bottomright"] = (Justification)(JustifyBottom() | JustifyRight());
        context.GetLua().set("Justify", tableJustification);
    }
};