#include "UI_Settings.h"

//temp
#include <windows.h>
#include <shobjidl.h>

void UI_Settings::renderLayer()
{
	ImGui::Begin(layerName.c_str());
	ImGui::Checkbox("Bloom", &useBloom);

	if (ImGui::SliderFloat("Gamma", &gamma, 0.0f, 5.0f))
	{
		Shader::activeShader->setFloat("gamma", gamma);
		//find way to access gamma and exposure in the FrameQuad drawFrameQuad function
	}

	if (ImGui::SliderFloat("Exposure", &exposure, 0.0f, 5.0f))
	{
		Shader::activeShader->setFloat("exposure", exposure);
	}

	if(useBloom)
	{
		if(ImGui::SliderFloat("Threshold", &threshold, 0.0f, 1.0f))
		{
			
		}
		if(ImGui::InputInt("Steps", &steps))
		{

		}
	}

	//TEST to see the loaded materials and textures
	if(ImGui::CollapsingHeader("Materials"))
	{
		std::vector<Material*>& materials = Scene::activeScene->materials;
		unsigned matCount = materials.size();

		for (size_t i = 0; i < matCount; i++)
		{
			if(ImGui::CollapsingHeader(materials[i]->getName().c_str()))
			{
				if(ImGui::CollapsingHeader("Diffuse texture"))
				{
					std::vector<Texture*>& textures = Scene::activeScene->textures;
					unsigned matCount = textures.size();

					for (size_t j = 0; j < matCount; j++)
					{
						if (ImGui::Selectable(textures[j]->path.c_str()))
						{
							materials[i]->setTexture(textures[j], 0);
						}
					}
					//ImGui::BeginCombo("test", "");

				}

				if (ImGui::CollapsingHeader("ORM texture"))
				{
					std::vector<Texture*>& textures = Scene::activeScene->textures;
					unsigned matCount = textures.size();

					for (size_t j = 0; j < matCount; j++)
					{
						if (ImGui::Selectable(textures[j]->path.c_str()))
						{
							materials[i]->setTexture(textures[j], 1);
						}
					}
				}

				if (ImGui::CollapsingHeader("Normal texture"))
				{
					std::vector<Texture*>& textures = Scene::activeScene->textures;
					unsigned matCount = textures.size();

					for (size_t j = 0; j < matCount; j++)
					{
						if (ImGui::Selectable(textures[j]->path.c_str()))
						{
							materials[i]->setTexture(textures[j], 2);
						}
					}
				}
			}
		}
	}

	//temp code for model importing
	if (ImGui::Button("Import Model"))
	{
		std::string path = OpenFolderDialog();
		//check path correctnes
		fileman->loadOBJ(path);
	}

	if (ImGui::Button("Import Texture"))
	{
		std::string path = OpenFolderDialog();
		std::vector<std::string> paths;
		paths.push_back(path);
		//check path correctnes
		fileman->loadTextures(paths);
	}

	ImGui::End();
}

UI_Settings* UI_Settings::clone()
{
	return new UI_Settings(*this);
}

UI_Settings::UI_Settings(FileManager* fileman) : UILayer("Settings"), steps(10), threshold(0.8), useBloom(false), gamma(1), exposure(1), fileman(fileman)
{

}


//TEMP CODE TESTING FILE BROWSING FOR IMPORT
std::string UI_Settings::OpenFolderDialog() const
{
	// Initialize COM
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	std::wstring filePath;

	if (SUCCEEDED(hr))
	{
		IFileDialog* pFileDialog = NULL;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileDialog, reinterpret_cast<void**>(&pFileDialog));

		if (SUCCEEDED(hr))
		{
			// Set the options for a file picker (default behavior)
			DWORD dwOptions;
			pFileDialog->GetOptions(&dwOptions);
			pFileDialog->SetOptions(dwOptions | FOS_FORCEFILESYSTEM); // Ensure we're working with files in the filesystem.

			// Optional: Set the file type filters
			// Define file types
			COMDLG_FILTERSPEC rgSpec[] =
			{
				//{ L"OBJ Files", L"*.obj" },
				{ L"All Files", L"*.*" }
			};
			// Set file types in the dialog (2 filters)
			pFileDialog->SetFileTypes(ARRAYSIZE(rgSpec), rgSpec);
			pFileDialog->SetFileTypeIndex(1); // Selects the second filter as the default
			pFileDialog->SetDefaultExtension(L"txt"); // Set a default extension

			// Show the dialog
			hr = pFileDialog->Show(NULL);

			// If the user selects a file
			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileDialog->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					// Retrieve the file path
					PWSTR pszFilePath = NULL;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Save the file path to a wstring.
					if (SUCCEEDED(hr))
					{
						filePath = pszFilePath;
						CoTaskMemFree(pszFilePath); // Free memory allocated for the path
					}
					pItem->Release();
				}
			}
			pFileDialog->Release();
		}
		CoUninitialize();
	}

	std::string str;
	size_t size;
	str.resize(filePath.length());
	wcstombs_s(&size, &str[0], str.size() + 1, filePath.c_str(), filePath.size());

	return str;
}
