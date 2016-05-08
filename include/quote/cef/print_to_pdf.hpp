#pragma once

#include "include/cef_client.h"
#include "include/cef_browser.h"
#include "include/wrapper/cef_helpers.h"

namespace quote{ namespace cef{

	namespace detail{
		template <typename PdfCallback, typename OnCancelled>
		class PrintToPdfClient :
			public CefPdfPrintCallback,
			public CefRunFileDialogCallback
		{
			IMPLEMENT_REFCOUNTING(PrintToPdfClient);

			CefRefPtr<CefBrowser> browser_;
			bool landscape_;
			PdfCallback callback_;
			OnCancelled cancelled_;

		public:
			PrintToPdfClient(
				CefRefPtr<CefBrowser> browser, bool landscape,
				PdfCallback callback, OnCancelled cancelled)
				: browser_(browser)
				, landscape_(landscape)
				, callback_(callback)
				, cancelled_(cancelled)
			{
				browser->GetHost()->RunFileDialog(
					static_cast<CefBrowserHost::FileDialogMode>(
						FILE_DIALOG_SAVE | FILE_DIALOG_OVERWRITEPROMPT_FLAG),
					L"Print to PDF",
					L"output.pdf",
					{ L".pdf" },
					0,
					this);
			}

			void OnFileDialogDismissed(
				int selected_accept_filter,
				const std::vector<CefString>& file_paths
				) override
			{
				if (!file_paths.empty()) {
					CefPdfPrintSettings settings;
					settings.header_footer_enabled = false;
					settings.landscape = (landscape_ ? 1 : 0);

					browser_->GetHost()->PrintToPDF(file_paths[0], settings, this);
				}
				else {
					cancelled_(browser_);
				}
			}

			void OnPdfPrintFinished(const CefString& path, bool ok) override
			{
				callback_(browser_, path, ok);
			}
		};

		struct print_to_pdf_do_nothing {
			void operator()(...)
			{

			}
		};
	}

	template <typename OnPdfFinished, typename OnCancelled = detail::print_to_pdf_do_nothing>
	void print_to_pdf(
		CefRefPtr<CefBrowser> browser,
		OnPdfFinished on_finished,
		bool landscape = false,
		OnCancelled on_cancelled = {}
	)
	{
		if (!::CefCurrentlyOn(TID_UI)) {
			::CefPostTask(
				TID_UI,
				::base::Bind(&print_to_pdf<OnPdfFinished, OnCancelled>, browser, on_finished, landscape, on_cancelled));
			return;
		}

		new detail::PrintToPdfClient<OnPdfFinished, OnCancelled>(
			browser, landscape,
			on_finished, on_cancelled);
	}

} }
