// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_SWHISTORY_SOULMETERFBS_HISTORY_H_
#define FLATBUFFERS_GENERATED_SWHISTORY_SOULMETERFBS_HISTORY_H_

#include "flatbuffers/flatbuffers.h"

#include "STRUCT_.h"
#include "SW_BUFF_.h"
#include "SW_PLAYER_METADATA_.h"
#include "SW_DAMAGE_PLAYER_.h"
#include "SW_COMBAT_.h"
#include "SW_PLOTINFO_.h"

namespace SoulMeterFBS {
namespace History {

struct _tDB2_Struct;
struct _tDB2_StructBuilder;

struct _tHistory;
struct _tHistoryBuilder;

struct _tDB2_Struct FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef _tDB2_StructBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT__ID = 4,
    VT__DB2 = 6,
    VT__TYPE = 8
  };
  uint32_t _id() const {
    return GetField<uint32_t>(VT__ID, 0);
  }
  uint32_t _db2() const {
    return GetField<uint32_t>(VT__DB2, 0);
  }
  int32_t _type() const {
    return GetField<int32_t>(VT__TYPE, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT__ID) &&
           VerifyField<uint32_t>(verifier, VT__DB2) &&
           VerifyField<int32_t>(verifier, VT__TYPE) &&
           verifier.EndTable();
  }
};

struct _tDB2_StructBuilder {
  typedef _tDB2_Struct Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add__id(uint32_t _id) {
    fbb_.AddElement<uint32_t>(_tDB2_Struct::VT__ID, _id, 0);
  }
  void add__db2(uint32_t _db2) {
    fbb_.AddElement<uint32_t>(_tDB2_Struct::VT__DB2, _db2, 0);
  }
  void add__type(int32_t _type) {
    fbb_.AddElement<int32_t>(_tDB2_Struct::VT__TYPE, _type, 0);
  }
  explicit _tDB2_StructBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<_tDB2_Struct> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<_tDB2_Struct>(end);
    return o;
  }
};

inline flatbuffers::Offset<_tDB2_Struct> Create_tDB2_Struct(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t _id = 0,
    uint32_t _db2 = 0,
    int32_t _type = 0) {
  _tDB2_StructBuilder builder_(_fbb);
  builder_.add__type(_type);
  builder_.add__db2(_db2);
  builder_.add__id(_id);
  return builder_.Finish();
}

struct _tHistory FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef _tHistoryBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT__WORD_ID = 4,
    VT__SAVE_TIME = 6,
    VT__TIME = 8,
    VT__MY_ID = 10,
    VT__EXT_INFO = 12,
    VT__DAMAGE_PLAYER = 14,
    VT__DB2_STRUCT = 16,
    VT__PLAYER_BUFF = 18,
    VT__PLOT_INFO = 20,
    VT__PLAYER_META = 22,
    VT__REAL_CLEAR_TIME = 24,
    VT__COMBAT = 26
  };
  uint32_t _word_id() const {
    return GetField<uint32_t>(VT__WORD_ID, 0);
  }
  const SoulMeterFBS::History::_sTime *_save_time() const {
    return GetStruct<const SoulMeterFBS::History::_sTime *>(VT__SAVE_TIME);
  }
  uint64_t _time() const {
    return GetField<uint64_t>(VT__TIME, 0);
  }
  uint32_t _my_id() const {
    return GetField<uint32_t>(VT__MY_ID, 0);
  }
  const flatbuffers::String *_ext_info() const {
    return GetPointer<const flatbuffers::String *>(VT__EXT_INFO);
  }
  const flatbuffers::Vector<flatbuffers::Offset<SoulMeterFBS::History::_tDamagePlayer>> *_damage_player() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<SoulMeterFBS::History::_tDamagePlayer>> *>(VT__DAMAGE_PLAYER);
  }
  const flatbuffers::Vector<flatbuffers::Offset<SoulMeterFBS::History::_tDB2_Struct>> *_db2_struct() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<SoulMeterFBS::History::_tDB2_Struct>> *>(VT__DB2_STRUCT);
  }
  const flatbuffers::Vector<flatbuffers::Offset<SoulMeterFBS::History::_tBuff>> *_player_buff() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<SoulMeterFBS::History::_tBuff>> *>(VT__PLAYER_BUFF);
  }
  const SoulMeterFBS::History::_tPlotInfo *_plot_info() const {
    return GetPointer<const SoulMeterFBS::History::_tPlotInfo *>(VT__PLOT_INFO);
  }
  const flatbuffers::Vector<flatbuffers::Offset<SoulMeterFBS::History::_tPlayerMetaData>> *_player_meta() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<SoulMeterFBS::History::_tPlayerMetaData>> *>(VT__PLAYER_META);
  }
  uint32_t _real_clear_time() const {
    return GetField<uint32_t>(VT__REAL_CLEAR_TIME, 0);
  }
  const SoulMeterFBS::History::_tCombatInterface *_combat() const {
    return GetPointer<const SoulMeterFBS::History::_tCombatInterface *>(VT__COMBAT);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT__WORD_ID) &&
           VerifyField<SoulMeterFBS::History::_sTime>(verifier, VT__SAVE_TIME) &&
           VerifyField<uint64_t>(verifier, VT__TIME) &&
           VerifyField<uint32_t>(verifier, VT__MY_ID) &&
           VerifyOffset(verifier, VT__EXT_INFO) &&
           verifier.VerifyString(_ext_info()) &&
           VerifyOffset(verifier, VT__DAMAGE_PLAYER) &&
           verifier.VerifyVector(_damage_player()) &&
           verifier.VerifyVectorOfTables(_damage_player()) &&
           VerifyOffset(verifier, VT__DB2_STRUCT) &&
           verifier.VerifyVector(_db2_struct()) &&
           verifier.VerifyVectorOfTables(_db2_struct()) &&
           VerifyOffset(verifier, VT__PLAYER_BUFF) &&
           verifier.VerifyVector(_player_buff()) &&
           verifier.VerifyVectorOfTables(_player_buff()) &&
           VerifyOffset(verifier, VT__PLOT_INFO) &&
           verifier.VerifyTable(_plot_info()) &&
           VerifyOffset(verifier, VT__PLAYER_META) &&
           verifier.VerifyVector(_player_meta()) &&
           verifier.VerifyVectorOfTables(_player_meta()) &&
           VerifyField<uint32_t>(verifier, VT__REAL_CLEAR_TIME) &&
           VerifyOffset(verifier, VT__COMBAT) &&
           verifier.VerifyTable(_combat()) &&
           verifier.EndTable();
  }
};

struct _tHistoryBuilder {
  typedef _tHistory Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add__word_id(uint32_t _word_id) {
    fbb_.AddElement<uint32_t>(_tHistory::VT__WORD_ID, _word_id, 0);
  }
  void add__save_time(const SoulMeterFBS::History::_sTime *_save_time) {
    fbb_.AddStruct(_tHistory::VT__SAVE_TIME, _save_time);
  }
  void add__time(uint64_t _time) {
    fbb_.AddElement<uint64_t>(_tHistory::VT__TIME, _time, 0);
  }
  void add__my_id(uint32_t _my_id) {
    fbb_.AddElement<uint32_t>(_tHistory::VT__MY_ID, _my_id, 0);
  }
  void add__ext_info(flatbuffers::Offset<flatbuffers::String> _ext_info) {
    fbb_.AddOffset(_tHistory::VT__EXT_INFO, _ext_info);
  }
  void add__damage_player(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<SoulMeterFBS::History::_tDamagePlayer>>> _damage_player) {
    fbb_.AddOffset(_tHistory::VT__DAMAGE_PLAYER, _damage_player);
  }
  void add__db2_struct(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<SoulMeterFBS::History::_tDB2_Struct>>> _db2_struct) {
    fbb_.AddOffset(_tHistory::VT__DB2_STRUCT, _db2_struct);
  }
  void add__player_buff(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<SoulMeterFBS::History::_tBuff>>> _player_buff) {
    fbb_.AddOffset(_tHistory::VT__PLAYER_BUFF, _player_buff);
  }
  void add__plot_info(flatbuffers::Offset<SoulMeterFBS::History::_tPlotInfo> _plot_info) {
    fbb_.AddOffset(_tHistory::VT__PLOT_INFO, _plot_info);
  }
  void add__player_meta(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<SoulMeterFBS::History::_tPlayerMetaData>>> _player_meta) {
    fbb_.AddOffset(_tHistory::VT__PLAYER_META, _player_meta);
  }
  void add__real_clear_time(uint32_t _real_clear_time) {
    fbb_.AddElement<uint32_t>(_tHistory::VT__REAL_CLEAR_TIME, _real_clear_time, 0);
  }
  void add__combat(flatbuffers::Offset<SoulMeterFBS::History::_tCombatInterface> _combat) {
    fbb_.AddOffset(_tHistory::VT__COMBAT, _combat);
  }
  explicit _tHistoryBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<_tHistory> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<_tHistory>(end);
    return o;
  }
};

inline flatbuffers::Offset<_tHistory> Create_tHistory(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t _word_id = 0,
    const SoulMeterFBS::History::_sTime *_save_time = 0,
    uint64_t _time = 0,
    uint32_t _my_id = 0,
    flatbuffers::Offset<flatbuffers::String> _ext_info = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<SoulMeterFBS::History::_tDamagePlayer>>> _damage_player = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<SoulMeterFBS::History::_tDB2_Struct>>> _db2_struct = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<SoulMeterFBS::History::_tBuff>>> _player_buff = 0,
    flatbuffers::Offset<SoulMeterFBS::History::_tPlotInfo> _plot_info = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<SoulMeterFBS::History::_tPlayerMetaData>>> _player_meta = 0,
    uint32_t _real_clear_time = 0,
    flatbuffers::Offset<SoulMeterFBS::History::_tCombatInterface> _combat = 0) {
  _tHistoryBuilder builder_(_fbb);
  builder_.add__time(_time);
  builder_.add__combat(_combat);
  builder_.add__real_clear_time(_real_clear_time);
  builder_.add__player_meta(_player_meta);
  builder_.add__plot_info(_plot_info);
  builder_.add__player_buff(_player_buff);
  builder_.add__db2_struct(_db2_struct);
  builder_.add__damage_player(_damage_player);
  builder_.add__ext_info(_ext_info);
  builder_.add__my_id(_my_id);
  builder_.add__save_time(_save_time);
  builder_.add__word_id(_word_id);
  return builder_.Finish();
}

inline flatbuffers::Offset<_tHistory> Create_tHistoryDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t _word_id = 0,
    const SoulMeterFBS::History::_sTime *_save_time = 0,
    uint64_t _time = 0,
    uint32_t _my_id = 0,
    const char *_ext_info = nullptr,
    const std::vector<flatbuffers::Offset<SoulMeterFBS::History::_tDamagePlayer>> *_damage_player = nullptr,
    const std::vector<flatbuffers::Offset<SoulMeterFBS::History::_tDB2_Struct>> *_db2_struct = nullptr,
    const std::vector<flatbuffers::Offset<SoulMeterFBS::History::_tBuff>> *_player_buff = nullptr,
    flatbuffers::Offset<SoulMeterFBS::History::_tPlotInfo> _plot_info = 0,
    const std::vector<flatbuffers::Offset<SoulMeterFBS::History::_tPlayerMetaData>> *_player_meta = nullptr,
    uint32_t _real_clear_time = 0,
    flatbuffers::Offset<SoulMeterFBS::History::_tCombatInterface> _combat = 0) {
  auto _ext_info__ = _ext_info ? _fbb.CreateString(_ext_info) : 0;
  auto _damage_player__ = _damage_player ? _fbb.CreateVector<flatbuffers::Offset<SoulMeterFBS::History::_tDamagePlayer>>(*_damage_player) : 0;
  auto _db2_struct__ = _db2_struct ? _fbb.CreateVector<flatbuffers::Offset<SoulMeterFBS::History::_tDB2_Struct>>(*_db2_struct) : 0;
  auto _player_buff__ = _player_buff ? _fbb.CreateVector<flatbuffers::Offset<SoulMeterFBS::History::_tBuff>>(*_player_buff) : 0;
  auto _player_meta__ = _player_meta ? _fbb.CreateVector<flatbuffers::Offset<SoulMeterFBS::History::_tPlayerMetaData>>(*_player_meta) : 0;
  return SoulMeterFBS::History::Create_tHistory(
      _fbb,
      _word_id,
      _save_time,
      _time,
      _my_id,
      _ext_info__,
      _damage_player__,
      _db2_struct__,
      _player_buff__,
      _plot_info,
      _player_meta__,
      _real_clear_time,
      _combat);
}

inline const SoulMeterFBS::History::_tHistory *Get_tHistory(const void *buf) {
  return flatbuffers::GetRoot<SoulMeterFBS::History::_tHistory>(buf);
}

inline const SoulMeterFBS::History::_tHistory *GetSizePrefixed_tHistory(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<SoulMeterFBS::History::_tHistory>(buf);
}

inline bool Verify_tHistoryBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<SoulMeterFBS::History::_tHistory>(nullptr);
}

inline bool VerifySizePrefixed_tHistoryBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<SoulMeterFBS::History::_tHistory>(nullptr);
}

inline void Finish_tHistoryBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<SoulMeterFBS::History::_tHistory> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixed_tHistoryBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<SoulMeterFBS::History::_tHistory> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace History
}  // namespace SoulMeterFBS

#endif  // FLATBUFFERS_GENERATED_SWHISTORY_SOULMETERFBS_HISTORY_H_
