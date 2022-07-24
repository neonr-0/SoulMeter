// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_SWPLAYERMETADATA_SOULMETERFBS_HISTORY_H_
#define FLATBUFFERS_GENERATED_SWPLAYERMETADATA_SOULMETERFBS_HISTORY_H_

#include "flatbuffers/flatbuffers.h"

namespace SoulMeterFBS {
namespace History {

struct _tPlayerMetaData;
struct _tPlayerMetaDataBuilder;

struct _tPlayerMetaData FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef _tPlayerMetaDataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT__ARMORBREAK = 4,
    VT__SG = 6,
    VT__STAMINA = 8,
    VT__SV = 10,
    VT__ATTACKSPEED = 12,
    VT__PARTIALDAMAGE = 14,
    VT__MAXHP = 16,
    VT__CURRENTHP = 18,
    VT__MAXATTACK = 20,
    VT__CRITDAMAGE = 22,
    VT__BOSSDAMAGE = 24,
    VT__ID = 26,
    VT__NAME = 28,
    VT__JOB = 30,
    VT__AVG_AB_SUM = 32,
    VT__AVG_AB_PREVIOUS_TIME = 34,
    VT__AVG_BD_SUM = 36,
    VT__AVG_BD_PREVIOUS_TIME = 38,
    VT__GEAR90_SUM = 40,
    VT__GEAR90_EFFECT_STARTED_TIME = 42,
    VT__GEAR50_SUM = 44,
    VT__GEAR50_EFFECT_STARTED_TIME = 46,
    VT__ACC01_SUM = 48,
    VT__ACC01_EFFECT_STARTED_TIME = 50,
    VT__ACC02_SUM = 52,
    VT__ACC02_EFFECT_STARTED_TIME = 54,
    VT__LOSEDHP = 56,
    VT__FULLAB_START_TIME = 58,
    VT__FULLAB_END_TIME = 60,
    VT__FULLAB_PREV_TIME = 62,
    VT__FULLAB_TIME = 64
  };
  float _armorbreak() const {
    return GetField<float>(VT__ARMORBREAK, 0.0f);
  }
  float _sg() const {
    return GetField<float>(VT__SG, 0.0f);
  }
  float _stamina() const {
    return GetField<float>(VT__STAMINA, 0.0f);
  }
  float _sv() const {
    return GetField<float>(VT__SV, 0.0f);
  }
  float _attackspeed() const {
    return GetField<float>(VT__ATTACKSPEED, 0.0f);
  }
  float _partialdamage() const {
    return GetField<float>(VT__PARTIALDAMAGE, 0.0f);
  }
  float _maxhp() const {
    return GetField<float>(VT__MAXHP, 0.0f);
  }
  float _currenthp() const {
    return GetField<float>(VT__CURRENTHP, 0.0f);
  }
  float _maxattack() const {
    return GetField<float>(VT__MAXATTACK, 0.0f);
  }
  float _critdamage() const {
    return GetField<float>(VT__CRITDAMAGE, 0.0f);
  }
  float _bossdamage() const {
    return GetField<float>(VT__BOSSDAMAGE, 0.0f);
  }
  uint32_t _id() const {
    return GetField<uint32_t>(VT__ID, 0);
  }
  const flatbuffers::String *_name() const {
    return GetPointer<const flatbuffers::String *>(VT__NAME);
  }
  int8_t _job() const {
    return GetField<int8_t>(VT__JOB, 0);
  }
  uint64_t _avg_ab_sum() const {
    return GetField<uint64_t>(VT__AVG_AB_SUM, 0);
  }
  uint64_t _avg_ab_previous_time() const {
    return GetField<uint64_t>(VT__AVG_AB_PREVIOUS_TIME, 0);
  }
  uint64_t _avg_bd_sum() const {
    return GetField<uint64_t>(VT__AVG_BD_SUM, 0);
  }
  uint64_t _avg_bd_previous_time() const {
    return GetField<uint64_t>(VT__AVG_BD_PREVIOUS_TIME, 0);
  }
  uint64_t _gear90_sum() const {
    return GetField<uint64_t>(VT__GEAR90_SUM, 0);
  }
  uint64_t _gear90_effect_started_time() const {
    return GetField<uint64_t>(VT__GEAR90_EFFECT_STARTED_TIME, 0);
  }
  uint64_t _gear50_sum() const {
    return GetField<uint64_t>(VT__GEAR50_SUM, 0);
  }
  uint64_t _gear50_effect_started_time() const {
    return GetField<uint64_t>(VT__GEAR50_EFFECT_STARTED_TIME, 0);
  }
  uint64_t _acc01_sum() const {
    return GetField<uint64_t>(VT__ACC01_SUM, 0);
  }
  uint64_t _acc01_effect_started_time() const {
    return GetField<uint64_t>(VT__ACC01_EFFECT_STARTED_TIME, 0);
  }
  uint64_t _acc02_sum() const {
    return GetField<uint64_t>(VT__ACC02_SUM, 0);
  }
  uint64_t _acc02_effect_started_time() const {
    return GetField<uint64_t>(VT__ACC02_EFFECT_STARTED_TIME, 0);
  }
  double _losedhp() const {
    return GetField<double>(VT__LOSEDHP, 0.0);
  }
  uint64_t _fullab_start_time() const {
    return GetField<uint64_t>(VT__FULLAB_START_TIME, 0);
  }
  uint64_t _fullab_end_time() const {
    return GetField<uint64_t>(VT__FULLAB_END_TIME, 0);
  }
  double _fullab_prev_time() const {
    return GetField<double>(VT__FULLAB_PREV_TIME, 0.0);
  }
  double _fullab_time() const {
    return GetField<double>(VT__FULLAB_TIME, 0.0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT__ARMORBREAK) &&
           VerifyField<float>(verifier, VT__SG) &&
           VerifyField<float>(verifier, VT__STAMINA) &&
           VerifyField<float>(verifier, VT__SV) &&
           VerifyField<float>(verifier, VT__ATTACKSPEED) &&
           VerifyField<float>(verifier, VT__PARTIALDAMAGE) &&
           VerifyField<float>(verifier, VT__MAXHP) &&
           VerifyField<float>(verifier, VT__CURRENTHP) &&
           VerifyField<float>(verifier, VT__MAXATTACK) &&
           VerifyField<float>(verifier, VT__CRITDAMAGE) &&
           VerifyField<float>(verifier, VT__BOSSDAMAGE) &&
           VerifyField<uint32_t>(verifier, VT__ID) &&
           VerifyOffset(verifier, VT__NAME) &&
           verifier.VerifyString(_name()) &&
           VerifyField<int8_t>(verifier, VT__JOB) &&
           VerifyField<uint64_t>(verifier, VT__AVG_AB_SUM) &&
           VerifyField<uint64_t>(verifier, VT__AVG_AB_PREVIOUS_TIME) &&
           VerifyField<uint64_t>(verifier, VT__AVG_BD_SUM) &&
           VerifyField<uint64_t>(verifier, VT__AVG_BD_PREVIOUS_TIME) &&
           VerifyField<uint64_t>(verifier, VT__GEAR90_SUM) &&
           VerifyField<uint64_t>(verifier, VT__GEAR90_EFFECT_STARTED_TIME) &&
           VerifyField<uint64_t>(verifier, VT__GEAR50_SUM) &&
           VerifyField<uint64_t>(verifier, VT__GEAR50_EFFECT_STARTED_TIME) &&
           VerifyField<uint64_t>(verifier, VT__ACC01_SUM) &&
           VerifyField<uint64_t>(verifier, VT__ACC01_EFFECT_STARTED_TIME) &&
           VerifyField<uint64_t>(verifier, VT__ACC02_SUM) &&
           VerifyField<uint64_t>(verifier, VT__ACC02_EFFECT_STARTED_TIME) &&
           VerifyField<double>(verifier, VT__LOSEDHP) &&
           VerifyField<uint64_t>(verifier, VT__FULLAB_START_TIME) &&
           VerifyField<uint64_t>(verifier, VT__FULLAB_END_TIME) &&
           VerifyField<double>(verifier, VT__FULLAB_PREV_TIME) &&
           VerifyField<double>(verifier, VT__FULLAB_TIME) &&
           verifier.EndTable();
  }
};

struct _tPlayerMetaDataBuilder {
  typedef _tPlayerMetaData Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add__armorbreak(float _armorbreak) {
    fbb_.AddElement<float>(_tPlayerMetaData::VT__ARMORBREAK, _armorbreak, 0.0f);
  }
  void add__sg(float _sg) {
    fbb_.AddElement<float>(_tPlayerMetaData::VT__SG, _sg, 0.0f);
  }
  void add__stamina(float _stamina) {
    fbb_.AddElement<float>(_tPlayerMetaData::VT__STAMINA, _stamina, 0.0f);
  }
  void add__sv(float _sv) {
    fbb_.AddElement<float>(_tPlayerMetaData::VT__SV, _sv, 0.0f);
  }
  void add__attackspeed(float _attackspeed) {
    fbb_.AddElement<float>(_tPlayerMetaData::VT__ATTACKSPEED, _attackspeed, 0.0f);
  }
  void add__partialdamage(float _partialdamage) {
    fbb_.AddElement<float>(_tPlayerMetaData::VT__PARTIALDAMAGE, _partialdamage, 0.0f);
  }
  void add__maxhp(float _maxhp) {
    fbb_.AddElement<float>(_tPlayerMetaData::VT__MAXHP, _maxhp, 0.0f);
  }
  void add__currenthp(float _currenthp) {
    fbb_.AddElement<float>(_tPlayerMetaData::VT__CURRENTHP, _currenthp, 0.0f);
  }
  void add__maxattack(float _maxattack) {
    fbb_.AddElement<float>(_tPlayerMetaData::VT__MAXATTACK, _maxattack, 0.0f);
  }
  void add__critdamage(float _critdamage) {
    fbb_.AddElement<float>(_tPlayerMetaData::VT__CRITDAMAGE, _critdamage, 0.0f);
  }
  void add__bossdamage(float _bossdamage) {
    fbb_.AddElement<float>(_tPlayerMetaData::VT__BOSSDAMAGE, _bossdamage, 0.0f);
  }
  void add__id(uint32_t _id) {
    fbb_.AddElement<uint32_t>(_tPlayerMetaData::VT__ID, _id, 0);
  }
  void add__name(flatbuffers::Offset<flatbuffers::String> _name) {
    fbb_.AddOffset(_tPlayerMetaData::VT__NAME, _name);
  }
  void add__job(int8_t _job) {
    fbb_.AddElement<int8_t>(_tPlayerMetaData::VT__JOB, _job, 0);
  }
  void add__avg_ab_sum(uint64_t _avg_ab_sum) {
    fbb_.AddElement<uint64_t>(_tPlayerMetaData::VT__AVG_AB_SUM, _avg_ab_sum, 0);
  }
  void add__avg_ab_previous_time(uint64_t _avg_ab_previous_time) {
    fbb_.AddElement<uint64_t>(_tPlayerMetaData::VT__AVG_AB_PREVIOUS_TIME, _avg_ab_previous_time, 0);
  }
  void add__avg_bd_sum(uint64_t _avg_bd_sum) {
    fbb_.AddElement<uint64_t>(_tPlayerMetaData::VT__AVG_BD_SUM, _avg_bd_sum, 0);
  }
  void add__avg_bd_previous_time(uint64_t _avg_bd_previous_time) {
    fbb_.AddElement<uint64_t>(_tPlayerMetaData::VT__AVG_BD_PREVIOUS_TIME, _avg_bd_previous_time, 0);
  }
  void add__gear90_sum(uint64_t _gear90_sum) {
    fbb_.AddElement<uint64_t>(_tPlayerMetaData::VT__GEAR90_SUM, _gear90_sum, 0);
  }
  void add__gear90_effect_started_time(uint64_t _gear90_effect_started_time) {
    fbb_.AddElement<uint64_t>(_tPlayerMetaData::VT__GEAR90_EFFECT_STARTED_TIME, _gear90_effect_started_time, 0);
  }
  void add__gear50_sum(uint64_t _gear50_sum) {
    fbb_.AddElement<uint64_t>(_tPlayerMetaData::VT__GEAR50_SUM, _gear50_sum, 0);
  }
  void add__gear50_effect_started_time(uint64_t _gear50_effect_started_time) {
    fbb_.AddElement<uint64_t>(_tPlayerMetaData::VT__GEAR50_EFFECT_STARTED_TIME, _gear50_effect_started_time, 0);
  }
  void add__acc01_sum(uint64_t _acc01_sum) {
    fbb_.AddElement<uint64_t>(_tPlayerMetaData::VT__ACC01_SUM, _acc01_sum, 0);
  }
  void add__acc01_effect_started_time(uint64_t _acc01_effect_started_time) {
    fbb_.AddElement<uint64_t>(_tPlayerMetaData::VT__ACC01_EFFECT_STARTED_TIME, _acc01_effect_started_time, 0);
  }
  void add__acc02_sum(uint64_t _acc02_sum) {
    fbb_.AddElement<uint64_t>(_tPlayerMetaData::VT__ACC02_SUM, _acc02_sum, 0);
  }
  void add__acc02_effect_started_time(uint64_t _acc02_effect_started_time) {
    fbb_.AddElement<uint64_t>(_tPlayerMetaData::VT__ACC02_EFFECT_STARTED_TIME, _acc02_effect_started_time, 0);
  }
  void add__losedhp(double _losedhp) {
    fbb_.AddElement<double>(_tPlayerMetaData::VT__LOSEDHP, _losedhp, 0.0);
  }
  void add__fullab_start_time(uint64_t _fullab_start_time) {
    fbb_.AddElement<uint64_t>(_tPlayerMetaData::VT__FULLAB_START_TIME, _fullab_start_time, 0);
  }
  void add__fullab_end_time(uint64_t _fullab_end_time) {
    fbb_.AddElement<uint64_t>(_tPlayerMetaData::VT__FULLAB_END_TIME, _fullab_end_time, 0);
  }
  void add__fullab_prev_time(double _fullab_prev_time) {
    fbb_.AddElement<double>(_tPlayerMetaData::VT__FULLAB_PREV_TIME, _fullab_prev_time, 0.0);
  }
  void add__fullab_time(double _fullab_time) {
    fbb_.AddElement<double>(_tPlayerMetaData::VT__FULLAB_TIME, _fullab_time, 0.0);
  }
  explicit _tPlayerMetaDataBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<_tPlayerMetaData> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<_tPlayerMetaData>(end);
    return o;
  }
};

inline flatbuffers::Offset<_tPlayerMetaData> Create_tPlayerMetaData(
    flatbuffers::FlatBufferBuilder &_fbb,
    float _armorbreak = 0.0f,
    float _sg = 0.0f,
    float _stamina = 0.0f,
    float _sv = 0.0f,
    float _attackspeed = 0.0f,
    float _partialdamage = 0.0f,
    float _maxhp = 0.0f,
    float _currenthp = 0.0f,
    float _maxattack = 0.0f,
    float _critdamage = 0.0f,
    float _bossdamage = 0.0f,
    uint32_t _id = 0,
    flatbuffers::Offset<flatbuffers::String> _name = 0,
    int8_t _job = 0,
    uint64_t _avg_ab_sum = 0,
    uint64_t _avg_ab_previous_time = 0,
    uint64_t _avg_bd_sum = 0,
    uint64_t _avg_bd_previous_time = 0,
    uint64_t _gear90_sum = 0,
    uint64_t _gear90_effect_started_time = 0,
    uint64_t _gear50_sum = 0,
    uint64_t _gear50_effect_started_time = 0,
    uint64_t _acc01_sum = 0,
    uint64_t _acc01_effect_started_time = 0,
    uint64_t _acc02_sum = 0,
    uint64_t _acc02_effect_started_time = 0,
    double _losedhp = 0.0,
    uint64_t _fullab_start_time = 0,
    uint64_t _fullab_end_time = 0,
    double _fullab_prev_time = 0.0,
    double _fullab_time = 0.0) {
  _tPlayerMetaDataBuilder builder_(_fbb);
  builder_.add__fullab_time(_fullab_time);
  builder_.add__fullab_prev_time(_fullab_prev_time);
  builder_.add__fullab_end_time(_fullab_end_time);
  builder_.add__fullab_start_time(_fullab_start_time);
  builder_.add__losedhp(_losedhp);
  builder_.add__acc02_effect_started_time(_acc02_effect_started_time);
  builder_.add__acc02_sum(_acc02_sum);
  builder_.add__acc01_effect_started_time(_acc01_effect_started_time);
  builder_.add__acc01_sum(_acc01_sum);
  builder_.add__gear50_effect_started_time(_gear50_effect_started_time);
  builder_.add__gear50_sum(_gear50_sum);
  builder_.add__gear90_effect_started_time(_gear90_effect_started_time);
  builder_.add__gear90_sum(_gear90_sum);
  builder_.add__avg_bd_previous_time(_avg_bd_previous_time);
  builder_.add__avg_bd_sum(_avg_bd_sum);
  builder_.add__avg_ab_previous_time(_avg_ab_previous_time);
  builder_.add__avg_ab_sum(_avg_ab_sum);
  builder_.add__name(_name);
  builder_.add__id(_id);
  builder_.add__bossdamage(_bossdamage);
  builder_.add__critdamage(_critdamage);
  builder_.add__maxattack(_maxattack);
  builder_.add__currenthp(_currenthp);
  builder_.add__maxhp(_maxhp);
  builder_.add__partialdamage(_partialdamage);
  builder_.add__attackspeed(_attackspeed);
  builder_.add__sv(_sv);
  builder_.add__stamina(_stamina);
  builder_.add__sg(_sg);
  builder_.add__armorbreak(_armorbreak);
  builder_.add__job(_job);
  return builder_.Finish();
}

inline flatbuffers::Offset<_tPlayerMetaData> Create_tPlayerMetaDataDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    float _armorbreak = 0.0f,
    float _sg = 0.0f,
    float _stamina = 0.0f,
    float _sv = 0.0f,
    float _attackspeed = 0.0f,
    float _partialdamage = 0.0f,
    float _maxhp = 0.0f,
    float _currenthp = 0.0f,
    float _maxattack = 0.0f,
    float _critdamage = 0.0f,
    float _bossdamage = 0.0f,
    uint32_t _id = 0,
    const char *_name = nullptr,
    int8_t _job = 0,
    uint64_t _avg_ab_sum = 0,
    uint64_t _avg_ab_previous_time = 0,
    uint64_t _avg_bd_sum = 0,
    uint64_t _avg_bd_previous_time = 0,
    uint64_t _gear90_sum = 0,
    uint64_t _gear90_effect_started_time = 0,
    uint64_t _gear50_sum = 0,
    uint64_t _gear50_effect_started_time = 0,
    uint64_t _acc01_sum = 0,
    uint64_t _acc01_effect_started_time = 0,
    uint64_t _acc02_sum = 0,
    uint64_t _acc02_effect_started_time = 0,
    double _losedhp = 0.0,
    uint64_t _fullab_start_time = 0,
    uint64_t _fullab_end_time = 0,
    double _fullab_prev_time = 0.0,
    double _fullab_time = 0.0) {
  auto _name__ = _name ? _fbb.CreateString(_name) : 0;
  return SoulMeterFBS::History::Create_tPlayerMetaData(
      _fbb,
      _armorbreak,
      _sg,
      _stamina,
      _sv,
      _attackspeed,
      _partialdamage,
      _maxhp,
      _currenthp,
      _maxattack,
      _critdamage,
      _bossdamage,
      _id,
      _name__,
      _job,
      _avg_ab_sum,
      _avg_ab_previous_time,
      _avg_bd_sum,
      _avg_bd_previous_time,
      _gear90_sum,
      _gear90_effect_started_time,
      _gear50_sum,
      _gear50_effect_started_time,
      _acc01_sum,
      _acc01_effect_started_time,
      _acc02_sum,
      _acc02_effect_started_time,
      _losedhp,
      _fullab_start_time,
      _fullab_end_time,
      _fullab_prev_time,
      _fullab_time);
}

inline const SoulMeterFBS::History::_tPlayerMetaData *Get_tPlayerMetaData(const void *buf) {
  return flatbuffers::GetRoot<SoulMeterFBS::History::_tPlayerMetaData>(buf);
}

inline const SoulMeterFBS::History::_tPlayerMetaData *GetSizePrefixed_tPlayerMetaData(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<SoulMeterFBS::History::_tPlayerMetaData>(buf);
}

inline bool Verify_tPlayerMetaDataBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<SoulMeterFBS::History::_tPlayerMetaData>(nullptr);
}

inline bool VerifySizePrefixed_tPlayerMetaDataBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<SoulMeterFBS::History::_tPlayerMetaData>(nullptr);
}

inline void Finish_tPlayerMetaDataBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<SoulMeterFBS::History::_tPlayerMetaData> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixed_tPlayerMetaDataBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<SoulMeterFBS::History::_tPlayerMetaData> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace History
}  // namespace SoulMeterFBS

#endif  // FLATBUFFERS_GENERATED_SWPLAYERMETADATA_SOULMETERFBS_HISTORY_H_
