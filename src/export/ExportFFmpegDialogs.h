/**********************************************************************

Audacity: A Digital Audio Editor

ExportFFmpegDialogs.h

LRN

**********************************************************************/

#if !defined(__EXPORT_FFMPEG_DIALOGS_H__)
#define __EXPORT_FFMPEG_DIALOGS_H__

#include "../Audacity.h"   // keep ffmpeg before wx because they interact // for USE_* macros

#if defined(USE_FFMPEG)

#include "../FFmpeg.h"     // and Audacity.h before FFmpeg for config*.h

#include "../xml/XMLFileReader.h"
#include "../FileNames.h"

#include <unordered_map>
#include "audacity/Types.h"

class wxArrayStringEx;

class wxArrayString;
class wxCheckBox;
class wxStaticText;
class wxTextCtrl;

/// Identifiers for pre-set export types.
enum FFmpegExposedFormat
{
   FMT_M4A,
   FMT_AC3,
   FMT_AMRNB,
   FMT_WMA2,
   FMT_OTHER,
   FMT_LAST
};

#define AV_CANMETA (AV_VERSION_INT(255, 255, 255))

/// Describes export type
struct ExposedFormat
{
   FFmpegExposedFormat fmtid; //!< one of the FFmpegExposedFormat
   const wxChar *name;        //!< format name (internal, should be unique; if not - export dialog may show unusual behaviour)
   const wxChar *extension;   //!< default extension for this format. More extensions may be added later via AddExtension.
   const wxChar *shortname;   //!< used to guess the format
   unsigned maxchannels;      //!< how many channels this format could handle
   const int canmetadata;           //!< !=0 if format supports metadata, AV_CANMETA any avformat version, otherwise version support added
   bool canutf8;              //!< true if format supports metadata in UTF-8, false otherwise
   const wxChar *description_; //!< format description (will be shown in export dialog) (untranslated!)
   AVCodecID codecid;         //!< codec ID (see libavcodec/avcodec.h)
   bool compiledIn;           //!< support for this codec/format is compiled in (checked at runtime)

   wxString Description() const; // get translation
};


/// Describes format-codec compatibility
struct CompatibilityEntry
{
   const wxChar *fmt; //!< format, recognizeable by guess_format()
   AVCodecID codec;   //!< codec ID
};


/// AC3 export options dialog
class ExportFFmpegAC3Options final : public wxPanelWrapper
{
public:

   ExportFFmpegAC3Options(wxWindow *parent, int format);
   virtual ~ExportFFmpegAC3Options();

   void PopulateOrExchange(ShuttleGui & S);
   bool TransferDataToWindow() override;
   bool TransferDataFromWindow() override;

   /// Bit Rates supported by AC3 encoder
   static const int iAC3BitRates[];
   /// Sample Rates supported by AC3 encoder (must end with zero-element)
   /// It is not used in dialog anymore, but will be required later
   static const int iAC3SampleRates[];

private:

   wxArrayStringEx mBitRateNames;
   std::vector<int>    mBitRateLabels;

   wxChoice *mBitRateChoice;
   int mBitRateFromChoice;
};

class ExportFFmpegAACOptions final : public wxPanelWrapper
{
public:

   ExportFFmpegAACOptions(wxWindow *parent, int format);
   virtual ~ExportFFmpegAACOptions();

   void PopulateOrExchange(ShuttleGui & S);
   bool TransferDataToWindow() override;
   bool TransferDataFromWindow() override;

private:

   wxSpinCtrl *mQualitySpin;
};

class ExportFFmpegAMRNBOptions final : public wxPanelWrapper
{
public:

   ExportFFmpegAMRNBOptions(wxWindow *parent, int format);
   virtual ~ExportFFmpegAMRNBOptions();

   void PopulateOrExchange(ShuttleGui & S);
   bool TransferDataToWindow() override;
   bool TransferDataFromWindow() override;

   static int iAMRNBBitRate[];

private:

   wxArrayStringEx mBitRateNames;
   std::vector<int>    mBitRateLabels;

   wxChoice *mBitRateChoice;
   int mBitRateFromChoice;
};

class ExportFFmpegWMAOptions final : public wxPanelWrapper
{
public:

   ExportFFmpegWMAOptions(wxWindow *parent, int format);
   ~ExportFFmpegWMAOptions();

   void PopulateOrExchange(ShuttleGui & S);
   bool TransferDataToWindow() override;
   bool TransferDataFromWindow() override;

   static const int iWMASampleRates[];
   static const int iWMABitRate[];

private:

   wxArrayStringEx mBitRateNames;
   std::vector<int>    mBitRateLabels;

   wxChoice *mBitRateChoice;
   int mBitRateFromChoice;
};

class ExportFFmpegCustomOptions final : public wxPanelWrapper
{
public:

   ExportFFmpegCustomOptions(wxWindow *parent, int format);
   ~ExportFFmpegCustomOptions();

   void PopulateOrExchange(ShuttleGui & S);
   bool TransferDataToWindow() override;
   bool TransferDataFromWindow() override;

   void OnOpen(wxCommandEvent & evt);

private:

   DECLARE_EVENT_TABLE()
};

/// Entry for the Applicability table
struct ApplicableFor
{
   bool                 enable;  //!< true if this control should be enabled, false otherwise
   int                  control; //!< control ID
   AVCodecID            codec;   //!< Codec ID
   const char          *format;  //!< Format short name
};

class FFmpegPresets;

/// Custom FFmpeg export dialog
class ExportFFmpegOptions final : public wxDialogWrapper
{
public:

   ExportFFmpegOptions(wxWindow *parent);
   ~ExportFFmpegOptions();
   void PopulateOrExchange(ShuttleGui & S);
   void OnOK(wxCommandEvent& event);
   void OnGetURL(wxCommandEvent& event);
   void OnFormatList(wxCommandEvent& event);
   void DoOnFormatList();
   void OnCodecList(wxCommandEvent& event);
   void DoOnCodecList();
   void OnAllFormats(wxCommandEvent& event);
   void OnAllCodecs(wxCommandEvent& event);
   void OnSavePreset(wxCommandEvent& event);
   void OnLoadPreset(wxCommandEvent& event);
   void OnDeletePreset(wxCommandEvent& event);
   void OnImportPresets(wxCommandEvent& event);
   void OnExportPresets(wxCommandEvent& event);
   bool SavePreset( bool bCheckForOverwrite);


   // Static tables
   static CompatibilityEntry CompatibilityList[];
   static int iAACProfileValues[];
   static ExposedFormat fmts[];
   static const int iAACSampleRates[];
   static ApplicableFor apptable[];

private:

   wxArrayString mShownFormatNames;
   wxArrayString mShownFormatLongNames;
   wxArrayString mShownCodecNames;
   wxArrayString mShownCodecLongNames;
   wxArrayStringEx mFormatNames;
   wxArrayString mFormatLongNames;
   wxArrayStringEx mCodecNames;
   wxArrayString mCodecLongNames;
   wxArrayStringEx mProfileNames;
   std::vector<int> mProfileLabels;
   wxArrayStringEx mPredictionOrderMethodNames;
   std::vector<int> mPredictionOrderMethodLabels;

   wxChoice *mFormatChoice;
   wxChoice *mCodecChoice;

   wxListBox *mFormatList;
   wxListBox *mCodecList;

   wxStaticText *mFormatName;
   wxStaticText *mCodecName;

   wxChoice *mPresetChoice;
   wxComboBox *mPresetCombo;
   wxSpinCtrl *mBitrateSpin;
   wxSpinCtrl *mQualitySpin;
   wxSpinCtrl *mSampleRateSpin;
   wxTextCtrl *mLanguageText;
   wxTextCtrl *mTag;
   wxSpinCtrl *mCutoffSpin;
   wxCheckBox *mBitReservoirCheck;
   wxChoice *mProfileChoice;
   //wxSpinCtrl *mTrellisSpin; //trellis is only applicable for ADPCM...scrap it.
   wxSpinCtrl *mCompressionLevelSpin;
   wxSpinCtrl *mFrameSizeSpin;
   wxCheckBox *mUseLPCCheck;
   wxSpinCtrl *mLPCCoeffsPrecisionSpin;
   wxSpinCtrl *mMinPredictionOrderSpin;
   wxSpinCtrl *mMaxPredictionOrderSpin;
   wxChoice *mPredictionOrderMethodChoice;
   wxSpinCtrl *mMinPartitionOrderSpin;
   wxSpinCtrl *mMaxPartitionOrderSpin;
   wxSpinCtrl *mMuxRate;
   wxSpinCtrl *mPacketSize;

   wxButton *mOk;
   wxButton *mSavePreset;
   wxButton *mLoadPreset;
   wxButton *mDeletePreset;
   wxButton *mImportPresets;
   wxButton *mExportPresets;

   int mBitRateFromChoice;
   int mSampleRateFromChoice;

   std::unique_ptr<FFmpegPresets> mPresets;

   wxArrayStringEx mPresetNames;

   /// Finds the format currently selected and returns it's name and description
   void FindSelectedFormat(wxString **name, wxString **longname);

   /// Finds the codec currently selected and returns it's name and description
   void FindSelectedCodec(wxString **name, wxString **longname);

   /// Retreives format list from libavformat
   void FetchFormatList();

   /// Retreives a list of formats compatible to codec
   ///\param id Codec ID
   ///\param selfmt format selected at the moment
   ///\return index of the selfmt in NEW format list or -1 if it is not in the list
   int FetchCompatibleFormatList(AVCodecID id, wxString *selfmt);

   /// Retreives codec list from libavcodec
   void FetchCodecList();

   /// Retreives a list of codecs compatible to format
   ///\param fmt Format short name
   ///\param id id of the codec selected at the moment
   ///\return index of the id in NEW codec list or -1 if it is not in the list
   int FetchCompatibleCodecList(const wxChar *fmt, AVCodecID id);

   /// Retreives list of presets from configuration file
   void FetchPresetList();

   // Enables/disables controls based on format/codec combination,
   // leaving only relevant controls enabled.
   // Hiding the controls may have been a better idea,
   // but it's hard to hide their text labels too
   void EnableDisableControls(AVCodec *cdc, wxString *selfmt);
   DECLARE_EVENT_TABLE()
};

//----------------------------------------------------------------------------
// FFmpegPresets
//----------------------------------------------------------------------------

class FFmpegPreset
{
public:
   FFmpegPreset();
   ~FFmpegPreset();

   wxString mPresetName;
   wxArrayString mControlState;

};

using FFmpegPresetMap = std::unordered_map<wxString, FFmpegPreset>;

class FFmpegPresets : XMLTagHandler
{
public:
   FFmpegPresets();
   ~FFmpegPresets();

   void GetPresetList(wxArrayString &list);
   void LoadPreset(ExportFFmpegOptions *parent, wxString &name);
   bool SavePreset(ExportFFmpegOptions *parent, wxString &name);
   void DeletePreset(wxString &name);
   bool OverwriteIsOk( wxString &name );
   FFmpegPreset *FindPreset(wxString &name);

   void ImportPresets(wxString &filename);
   void ExportPresets(wxString &filename);

   bool HandleXMLTag(const wxChar *tag, const wxChar **attrs) override;
   XMLTagHandler *HandleXMLChild(const wxChar *tag) override;
   void WriteXMLHeader(XMLWriter &xmlFile) const;
   void WriteXML(XMLWriter &xmlFile) const;

private:

   FFmpegPresetMap mPresets;
   FFmpegPreset *mPreset; // valid during XML parsing only
   bool mAbortImport; // tells importer to ignore the rest of the import
};

#endif

#endif //__EXPORT_FFMPEG_DIALOGS_H__
